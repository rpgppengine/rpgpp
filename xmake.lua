

includes("toolchain/*.lua")
includes("packages/*.lua")
if is_plat("switch") then
	set_toolchains("devkita64")
	set_arch("arm64")
end

add_requires("raylib", "tgui", "nlohmann_json", "luajit", "pugixml")
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

target("rpgpp")
set_plat(os.host())
set_arch(os.arch())
set_kind("static")
add_packages("raylib", "nlohmann_json", "luajit", "pugixml")
set_languages("cxx17")
add_includedirs("include/", "include/lua/", "include/ui_elements/")
add_files("src/*.cpp", "src/lua/*.cpp", "src/ui_elements/*.cpp")
add_defines("SOL_LUAJIT")
if is_plat("linux") then
	add_cxxflags("-fPIC")
end

-- !!!!!!! FIX MSVC ISSUE !!!!!!!
--
-- ATTENTION! The RPG++ LUA SOURCES ARE NOT INCLUDED YET. THIS IS CAUSING LINKER ISSUES!
-- ADD THEM TO THE PROJECT, THEN UNCOMMENT THIS PART!
--
target("rpgpplua")
set_kind("shared")
set_languages("cxx17")
add_includedirs("include/")
add_files("src/rpgpplua/*.cpp")
add_packages("nlohmann_json", "raylib", "luajit", { public = true })
add_deps("rpgpp")

target("game")
set_kind("binary")
add_deps("rpgpp")
-- for some very cool bizarre reason, xmake didn't compile the game target without these.
add_packages("nlohmann_json", "raylib", "luajit")
set_languages("cxx17")
add_includedirs("include/")
add_files("src/game/main.cpp")

task("check_translation")
on_run( function ()
	import("tools.translation_checker.checker")
	checker.Main()
end)

set_menu {
	usage = "xmake check_translation",
	description = "Check the project's translation status."
}

task("build_doc")
on_run( function ()
	os.mkdir("build/")
	os.execv("doxygen", { "doxygen.conf" })
	os.execv("breathe-apidoc", { "-o", "docs/Dev", "-m", "-f", "build/doxygen/xml" })
	os.execv("make", { "html" })
end)

set_menu {
	usage = "xmake build_doc",
	description = "Build the project's documentation."
}

target("editor")
-- add_includedirs("include/", "include/editor/", "libs/raylib/src/", "libs/tgui/include/")
-- add_includedirs("libs/raylib/src/", "libs/tgui/include/")
if is_plat("windows") then
	add_defines("TGUI_STATIC")
	-- Add the actual reference to this file, if necessary.
	-- Since it doesn't work on my environment, it was removed.
	-- If you need to add it, uncomment this part!
	--
	-- add_links("tgui-s-d")
end
-- add_links("tree-sitter-lua", { kind = "static" })
set_kind("binary")
-- FIX FOR MSVC !!!!
-- NOTE: DO NOT DELETE THIS LINE
if is_plat("windows") then
	add_cxxflags("/permissive-")
end
set_languages("cxx17")
add_includedirs("include/", "include/editor/", os.dirs(path.join(os.scriptdir())))
add_files("src/editor/**.cpp", "src/edui/**.cpp")
add_deps("rpgpp")
add_packages("raylib", "tgui", "nlohmann_json", "luajit", "pugixml")
after_build( function (target)
	os.cp("$(curdir)/resources", "./build/$(plat)/$(arch)/$(mode)/", { async = true })
	if is_plat("linux", "macosx") then
		os.cp("./build/$(plat)/$(arch)/$(mode)/librpgpp.a", "$(curdir)/game-src/lib/librpgpp.a", { async = true })
		os.cp("./build/$(plat)/$(arch)/$(mode)/librpgpplua.so", "$(curdir)/game-src/lib/librpgpplua.so",
			{ async = true })
		os.cp(path.join(target:pkg("luajit"):installdir(), "bin/luajit*"), "$(curdir)/execs/luajit", { async = true })
	end
	if is_plat("windows") then
		os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.lib", "$(curdir)/game-src/lib/rpgpplua.lib", { async = true })
		os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.dll", "$(curdir)/game-src/lib/rpgpplua.dll", { async = true })
		os.cp(path.join(target:pkg("luajit"):installdir(), "bin/luajit.exe"), "$(curdir)/execs/luajit.exe",
			{ async = true })
	end
	os.cp("$(curdir)/game-src", "./build/$(plat)/$(arch)/$(mode)/", { async = true })
	os.cp("$(curdir)/execs", "./build/$(plat)/$(arch)/$(mode)/", { async = true })

	os.rm("$(curdir)/game-src", { async = true })
	os.rm("$(curdir)/execs", { async = true })
	-- remove this line to test if the configuration file changes
	os.cp("$(curdir)/rpgpp.ini", "./build/$(plat)/$(arch)/$(mode)/", { copy_if_different = true })
	print("Pro tip: Check whether translations are up to date with `xmake check_translation`!")
	print("Environment:")
	print("  builddir: " .. "$(builddir)")
	print("  plat: " .. "$(plat)")
	print("  arch: " .. "$(arch)")
	print("  mode: " .. "$(mode)")
end)
