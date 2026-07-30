package("raylib")
    set_homepage("http://www.raylib.com")
    set_description("A simple and easy-to-use library to enjoy videogames programming.")
    set_license("zlib")

    if is_plat("macosx") and is_arch("x86_64") then
        add_urls("https://github.com/luizpestana/raylib-nx/releases/download/$(version).tar.gz", {version = function (version)
            if version:ge("3.5.0") then
                return version .. "/raylib-" .. version .. "_macos"
            else
                return version .. "/raylib-" .. version .. "-macOS"
            end
        end})
        add_versions("6.0", "aa030655796637f14d3e66cb9b64bef136ed4ef7")
    else
        add_urls("https://github.com/luizpestana/raylib-nx/archive/$(version).tar.gz",
                 "https://github.com/luizpestana/raylib-nx.git")
        add_versions("6.0", "aa030655796637f14d3e66cb9b64bef136ed4ef7")
    end

    if not (is_plat("macosx") and is_arch("x86_64")) then
        add_deps("cmake >=3.11")
    end

    if is_plat("macosx") then
        add_frameworks("CoreVideo", "CoreGraphics", "AppKit", "IOKit", "CoreFoundation", "Foundation")
    elseif is_plat("windows", "mingw") then
        add_syslinks("gdi32", "user32", "winmm", "shell32")
    elseif is_plat("linux") then
        add_syslinks("pthread", "dl", "m")
        add_deps("libx11", "libxrandr", "libxrender", "libxinerama", "libxcursor", "libxi", "libxfixes", "libxext")
    elseif is_plat("wasm") then
        add_ldflags("-sUSE_GLFW=3", "-sASSERTIONS=1", "-sWASM=1", "-sASYNCIFY", "-sGL_ENABLE_GET_PROC_ADDRESS=1", {force = true})
    elseif is_plat("android") then
        add_syslinks("log", "android", "EGL", "GLESv2", "OpenSLES", "m")
        add_ldflags("-Wl,--wrap=fopen")
    end
    add_deps("opengl", {optional = true})

    on_install("macosx|x86_64", function (package)
        os.cp("include/*.h", package:installdir("include"))
        os.cp("lib/libraylib.a", package:installdir("lib"))
    end)

    on_install("windows", "linux", "macosx|arm64", "mingw", "wasm", "android", function (package)
        local configs = {"-DBUILD_EXAMPLES=OFF"}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:is_debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        if package:is_plat("wasm") then
            table.insert(configs, "-DPLATFORM=Web")
        elseif package:is_plat("android") then
            table.insert(configs, "-DPLATFORM=Android")
            table.insert(configs, "-DANDROID_ABI=" .. (package:arch() or "arm64-v8a"))
            table.insert(configs, "-DOPENGL_API=ES2")
            table.insert(configs, "-DUSE_EXTERNAL_GLFW=OFF")
        end
        import("package.tools.cmake").install(package, configs, {packagedeps = {"libx11", "libxrender", "libxrandr", "libxinerama", "libxcursor", "libxi", "libxfixes", "libxext"}})
    end)

    on_install("cross", function (package)
    	local configs = {"PLATFORM=PLATFORM_NX"}
     	os.cd("src")
    	import("package.tools.make").build(package, configs)
     	configs = {}
      	table.insert(configs, "install")
      	table.insert(configs, "DESTDIR=" .. package:installdir());
        os.vrun("make", configs)

    	--os.vrun("/opt/devkitpro/portlibs/switch/bin/aarch64-none-elf-cmake -DPLATFORM=NX -DCMAKE_BUILD_TYPE=Release -S . -B cmake-build-switch")
    end)
package_end()
