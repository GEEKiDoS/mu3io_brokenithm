workspace "mu3io_brokenithm"
	location "./build"
	objdir "%{wks.location}/obj"
	targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

	architecture "x64"
	platforms "x64"

	configurations {
		"Debug",
		"Release",
	}

	buildoptions "/std:c++latest"
	systemversion "latest"
	symbols "On"
	staticruntime "On"
	editandcontinue "Off"
	warnings "Off"
	characterset "ASCII"

	flags {
		"NoIncrementalLink",
		"NoMinimalRebuild",
		"MultiProcessorCompile",
	}

	staticruntime "Off"

	configuration "Release"
		optimize "Full"
		buildoptions "/Os"

	configuration "Debug"
		optimize "Debug"

	project "mu3io_brokenithm"
		targetname "mu3io_brokenithm"

		language "C++"
		kind "SharedLib"

		files {
			"./src/**.hpp",
			"./src/**.cpp",
			"./src/**.def",
		}

		includedirs {
			"./src",
			"%{prj.location}/src",
		}

		links {
			"dinput8.lib",
			"dxguid.lib",
		}

		configuration "Release"
			linkoptions "/SAFESEH:NO"
			syslibdirs {
				"./libs/Release",
			}

		configuration "Debug"
			linkoptions "/SAFESEH:NO"
			syslibdirs {
				"./libs/Debug",
			}
