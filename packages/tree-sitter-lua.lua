package("tree-sitter-lua")
	add_deps("cmake")
	set_homepage("https://github.com/tree-sitter-grammars/tree-sitter-lua")
    set_description("Lua grammar for tree-sitter.")
    set_license("mit")

    set_urls("https://github.com/tree-sitter-grammars/tree-sitter-lua/releases/download/v$(version)/tree-sitter-lua.tar.gz")

    add_versions("0.5.0", "dd6d995634ea7dc2eac687f4890f995f03c421e0c42c34b3cfcfc81c47e48d7e")

    on_install(function(package)
    	os.cp(path.join(package:scriptdir(), "packages", "ts-lua-inner.lua"), "xmake.lua")
		import("package.tools.xmake").install(package)
    end)
