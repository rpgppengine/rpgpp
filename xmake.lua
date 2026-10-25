includes("toolchain/*.lua")
includes("packages/*.lua")
if is_plat("switch") then
	set_toolchains("devkita64")
	set_arch("arm64")
end

add_requires("raylib", "nlohmann_json", "luajit", "pugixml", "tree-sitter", "tree-sitter-lua")
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

if is_plat("linux", "mingw") then
	add_cxxflags("-fPIC")
end
if is_plat("mingw") then
	add_cxxflags("-Wa,-mbig-obj")
	add_ldflags("-static", "-static-libgcc", "-static-libstdc++")
end

target("rpgpp")
set_kind("static")
add_packages("raylib", "nlohmann_json", "luajit", "pugixml", { public = true })
set_languages("cxx17")
add_includedirs("include/", "include/lua/", "include/ui_elements/")
add_files("src/*.cpp", "src/lua/*.cpp", "src/ui_elements/*.cpp")
add_defines("SOL_LUAJIT")

target("rpgpplua")
set_kind("shared")
set_languages("cxx17")
add_deps("rpgpp")
add_includedirs("include/")
add_files("src/rpgpplua/*.cpp")

target("game")
set_kind("binary")
add_deps("rpgpp")
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

target("edui")
set_kind("static")
set_languages("cxx17")
add_packages("raylib", "tree-sitter", "tree-sitter-lua", { public = true })
add_includedirs("include/", "include/edui/")
add_files("src/edui/**.cpp")

target("editor")
set_kind("binary")
if is_plat("windows") then
	add_cxxflags("/permissive-")
end
set_languages("cxx17")
add_includedirs("include/", "include/editor-new/")
add_files("src/editor-new/**.cpp")
add_deps("rpgpp", "edui")
add_packages("raylib", "nlohmann_json", "luajit", "pugixml", "tree-sitter", "tree-sitter-lua")
after_build( function (target)
	os.cp("$(curdir)/resources", "./build/$(plat)/$(arch)/$(mode)/", { async = true })
	if is_plat("linux", "macosx", "mingw") then
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
