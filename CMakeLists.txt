cmake_minimum_required (VERSION 3.6)

set (VSE_VERSION_1 0)
set (VSE_VERSION_2 5)
set (VSE_VERSION_3 6)
set (VSE_LIB_NAME VisualScriptEngine)
set (VSE_CUSTOM_BUILD_OPTIONS ${VSE_CUSTOM_BUILD_OPTIONS})

function (SetCompilerOptions module)
	target_compile_options (${module} PUBLIC "$<$<CONFIG:Debug>:-DDEBUG>")
	if (WIN32)
		set (AdditionalWarnings /w44061 /w44062 /w44265 /w44266 /w44355 /w44596 /w44800)
		target_compile_options (${module} PUBLIC /W4 /WX ${AdditionalWarnings} ${VSE_CUSTOM_BUILD_OPTIONS})
	else ()
		target_compile_options (${module} PUBLIC -std=c++11 -Wall -Wextra -Werror ${VSE_CUSTOM_BUILD_OPTIONS})
	endif ()
endfunction ()

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)
set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/DevKit CACHE PATH "Install prefix.")
set (CMAKE_DEBUG_POSTFIX "Debug")

add_definitions (-DUNICODE -D_UNICODE)

project (VisualScriptEngine)

enable_testing ()

# NodeEngine

set (NodeEngineSourcesFolder Sources/NodeEngine)
file (GLOB NodeEngineHeaderFiles ${NodeEngineSourcesFolder}/*.hpp)
file (GLOB NodeEngineSourceFiles ${NodeEngineSourcesFolder}/*.cpp)
set (
	NodeEngineFiles
	${NodeEngineHeaderFiles}
	${NodeEngineSourceFiles}
)
source_group ("Sources" FILES ${NodeEngineFiles})
add_library (NodeEngine STATIC ${NodeEngineFiles})
set_target_properties (NodeEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_include_directories (NodeEngine PUBLIC ${NodeEngineSourcesFolder})
SetCompilerOptions (NodeEngine)
install (TARGETS NodeEngine DESTINATION lib)
install (FILES ${NodeEngineHeaderFiles} DESTINATION include)
install (FILES ${NodeEngineSourceFiles} DESTINATION source)

# NodeUIEngine

set (NodeUIEngineSourcesFolder Sources/NodeUIEngine)
configure_file (${NodeUIEngineSourcesFolder}/NUIE_VersionInfo.hpp.in ${PROJECT_BINARY_DIR}/NUIE_VersionInfo.hpp)
include_directories (${PROJECT_BINARY_DIR})
file (GLOB NodeUIEngineHeaderFiles ${NodeUIEngineSourcesFolder}/*.hpp ${PROJECT_BINARY_DIR}/NUIE_VersionInfo.hpp)
file (GLOB NodeUIEngineSourceFiles ${NodeUIEngineSourcesFolder}/*.cpp)
set (
	NodeUIEngineFiles
	${NodeUIEngineHeaderFiles}
	${NodeUIEngineSourceFiles}
)
source_group ("Sources" FILES ${NodeUIEngineFiles})
add_library (NodeUIEngine STATIC ${NodeUIEngineFiles})
set_target_properties (NodeUIEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_include_directories (NodeUIEngine PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder})
target_link_libraries (NodeUIEngine NodeEngine)
SetCompilerOptions (NodeUIEngine)
install (TARGETS NodeUIEngine DESTINATION lib)
install (FILES ${NodeUIEngineHeaderFiles} DESTINATION include)
install (FILES ${NodeUIEngineSourceFiles} DESTINATION source)

# BuiltInNodes

set (BuiltInNodesSourcesFolder Sources/BuiltInNodes)
file (GLOB BuiltInNodesHeaderFiles ${BuiltInNodesSourcesFolder}/*.hpp)
file (GLOB BuiltInNodesSourceFiles ${BuiltInNodesSourcesFolder}/*.cpp)
set (
	BuiltInNodesFiles
	${BuiltInNodesHeaderFiles}
	${BuiltInNodesSourceFiles}
)
source_group ("Sources" FILES ${BuiltInNodesFiles})
add_library (BuiltInNodes STATIC ${BuiltInNodesFiles})
set_target_properties (BuiltInNodes PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_include_directories (BuiltInNodes PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder} ${BuiltInNodesSourcesFolder})
target_link_libraries (BuiltInNodes NodeEngine NodeUIEngine)
SetCompilerOptions (BuiltInNodes)
install (TARGETS BuiltInNodes DESTINATION lib)
install (FILES ${BuiltInNodesHeaderFiles} DESTINATION include)
install (FILES ${BuiltInNodesSourceFiles} DESTINATION source)

# Localization

set (LocalizationSourcesFolder Sources/Localization)
file (GLOB LocalizationFiles ${LocalizationSourcesFolder}/*.po)
install (FILES ${LocalizationFiles} DESTINATION localization)

# NodeEngineTest

set (TestFrameworkSourcesFolder Sources/TestFramework)
set (TestEnvironmentSourcesFolder Sources/NodeEngineTest/TestEnvironment)
set (NodeEngineTestSourcesFolder Sources/NodeEngineTest)
file (GLOB TestFrameworkFiles ${TestFrameworkSourcesFolder}/*.hpp ${TestFrameworkSourcesFolder}/*.cpp)
file (GLOB TestEnvironmentFiles ${TestEnvironmentSourcesFolder}/*.hpp ${TestEnvironmentSourcesFolder}/*.cpp)
file (GLOB NodeEngineTestHeaderFiles ${NodeEngineTestSourcesFolder}/*.hpp)
file (GLOB NodeEngineTestSourceFiles ${NodeEngineTestSourcesFolder}/*.cpp)
set (
	NodeEngineTestTestFiles
	${NodeEngineTestHeaderFiles}
	${NodeEngineTestSourceFiles}
)
set (
	NodeEngineTestFiles
	${TestFrameworkFiles}
	${TestEnvironmentFiles}
	${NodeEngineTestTestFiles}
)
source_group ("Framework" FILES ${TestFrameworkFiles})
source_group ("Environment" FILES ${TestEnvironmentFiles})
source_group ("Sources" FILES ${NodeEngineTestTestFiles})
add_executable (NodeEngineTest ${NodeEngineTestFiles})
set_target_properties (NodeEngineTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_include_directories (
	NodeEngineTest PUBLIC
	${NodeEngineSourcesFolder}
	${NodeUIEngineSourcesFolder}
	${BuiltInNodesSourcesFolder}
	${TestFrameworkSourcesFolder}
	${TestEnvironmentSourcesFolder}
)
target_link_libraries (NodeEngineTest NodeEngine NodeUIEngine BuiltInNodes)
get_filename_component (NodeEngineTestSourcesFolderAbsolute "${CMAKE_CURRENT_LIST_DIR}/${NodeEngineTestSourcesFolder}" ABSOLUTE)
add_custom_command (TARGET NodeEngineTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${NodeEngineTestSourcesFolderAbsolute}/VisualTestFiles $<TARGET_FILE_DIR:NodeEngineTest>/VisualTestFiles
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${NodeEngineTestSourcesFolderAbsolute}/CompatibilityTestFiles $<TARGET_FILE_DIR:NodeEngineTest>/CompatibilityTestFiles
)
SetCompilerOptions (NodeEngineTest)
add_test (NodeEngineTest NodeEngineTest)

# EmbeddingTutorial

set (EmbeddingTutorialSourcesFolder Sources/EmbeddingTutorial)
file (GLOB EmbeddingTutorialHeaderFiles ${EmbeddingTutorialSourcesFolder}/*.hpp)
file (GLOB EmbeddingTutorialSourceFiles ${EmbeddingTutorialSourcesFolder}/*.cpp)
set (
	EmbeddingTutorialFiles
	${EmbeddingTutorialHeaderFiles}
	${EmbeddingTutorialSourceFiles}
)
source_group ("Sources" FILES ${EmbeddingTutorialFiles})
add_executable (EmbeddingTutorial ${EmbeddingTutorialFiles})
set_target_properties (EmbeddingTutorial PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_include_directories (
	EmbeddingTutorial PUBLIC
	${NodeEngineSourcesFolder}
	${NodeUIEngineSourcesFolder}
	${BuiltInNodesSourcesFolder}
)
target_link_libraries (EmbeddingTutorial NodeEngine NodeUIEngine BuiltInNodes)
SetCompilerOptions (EmbeddingTutorial)
if (WIN32)
	target_compile_options (EmbeddingTutorial PUBLIC /wd4100)
else ()
	target_compile_options (EmbeddingTutorial PUBLIC -Wno-unused-parameter)
endif ()

if (WIN32)

	# WindowsAppSupport

	set (WindowsAppSupportSourcesFolder Sources/WindowsAppSupport)
	file (GLOB WindowsAppSupportHeaderFiles ${WindowsAppSupportSourcesFolder}/*.hpp)
	file (GLOB WindowsAppSupportSourceFiles ${WindowsAppSupportSourcesFolder}/*.cpp)
	set (
		WindowsAppSupportFiles
		${WindowsAppSupportHeaderFiles}
		${WindowsAppSupportSourceFiles}
	)
	source_group ("Sources" FILES ${WindowsAppSupportFiles})
	add_library (WindowsAppSupport STATIC ${WindowsAppSupportFiles})
	set_target_properties (WindowsAppSupport PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
	target_include_directories (WindowsAppSupport PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder} ${WindowsAppSupportSourcesFolder})
	target_link_libraries (WindowsAppSupport NodeEngine NodeUIEngine)
	SetCompilerOptions (WindowsAppSupport)
	install (TARGETS WindowsAppSupport DESTINATION lib)
	install (FILES ${WindowsAppSupportHeaderFiles} DESTINATION include)
	install (FILES ${WindowsAppSupportSourceFiles} DESTINATION source)

	# WindowsReferenceApp

	set (WindowsReferenceAppSourcesFolder Sources/WindowsReferenceApp)
	file (GLOB WindowsReferenceAppHeaderFiles ${WindowsReferenceAppSourcesFolder}/*.hpp)
	file (GLOB WindowsReferenceAppSourceFiles
		${WindowsReferenceAppSourcesFolder}/*.cpp
		${WindowsReferenceAppSourcesFolder}/*.rc
		${WindowsReferenceAppSourcesFolder}/*.manifest
	)
	set (
		WindowsReferenceAppFiles
		${WindowsReferenceAppHeaderFiles}
		${WindowsReferenceAppSourceFiles}
	)
	source_group ("Sources" FILES ${WindowsReferenceAppFiles})
	add_executable (WindowsReferenceApp WIN32 ${WindowsReferenceAppFiles})
	set_target_properties (WindowsReferenceApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
	target_include_directories (
		WindowsReferenceApp PUBLIC
		${NodeEngineSourcesFolder}
		${NodeUIEngineSourcesFolder}
		${BuiltInNodesSourcesFolder}
		${WindowsAppSupportSourcesFolder}
	)
	target_link_libraries (WindowsReferenceApp NodeEngine NodeUIEngine BuiltInNodes WindowsAppSupport)
	SetCompilerOptions (WindowsReferenceApp)
	
	# WindowsEmbeddingDemo

	set (WindowsEmbeddingDemoSourcesFolder Sources/WindowsEmbeddingDemo)
	file (GLOB WindowsEmbeddingDemoHeaderFiles ${WindowsEmbeddingDemoSourcesFolder}/*.hpp)
	file (GLOB WindowsEmbeddingDemoSourceFiles ${WindowsEmbeddingDemoSourcesFolder}/*.cpp)
	set (
		WindowsEmbeddingDemoFiles
		${WindowsEmbeddingDemoHeaderFiles}
		${WindowsEmbeddingDemoSourceFiles}
	)
	source_group ("Sources" FILES ${WindowsEmbeddingDemoFiles})
	add_executable (WindowsEmbeddingDemo WIN32 ${WindowsEmbeddingDemoFiles})
	set_target_properties (WindowsEmbeddingDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
	target_include_directories (
		WindowsEmbeddingDemo PUBLIC
		${NodeEngineSourcesFolder}
		${NodeUIEngineSourcesFolder}
		${BuiltInNodesSourcesFolder}
		${WindowsAppSupportSourcesFolder}
	)
	target_link_libraries (WindowsEmbeddingDemo NodeEngine NodeUIEngine BuiltInNodes WindowsAppSupport)
	SetCompilerOptions (WindowsEmbeddingDemo)

endif ()	

if (APPLE)

	find_library (CocoaFramework Cocoa)

	# MacOSAppSupport

	set (MacOSAppSupportSourcesFolder Sources/MacOSAppSupport)
	file (GLOB MacOSAppSupportHeaderFiles ${MacOSAppSupportSourcesFolder}/*.hpp)
	file (GLOB MacOSAppSupportSourceFiles ${MacOSAppSupportSourcesFolder}/*.cpp ${MacOSAppSupportSourcesFolder}/*.mm)
	set (
		MacOSAppSupportFiles
		${MacOSAppSupportHeaderFiles}
		${MacOSAppSupportSourceFiles}
	)
	source_group ("Sources" FILES ${MacOSAppSupportFiles})
	add_library (MacOSAppSupport STATIC ${MacOSAppSupportFiles})
	set_target_properties (MacOSAppSupport PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
	target_include_directories (MacOSAppSupport PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder} ${MacOSAppSupportSourcesFolder})
	target_link_libraries (MacOSAppSupport ${CocoaFramework})
	target_link_libraries (MacOSAppSupport NodeEngine NodeUIEngine)
	SetCompilerOptions (MacOSAppSupport)
	install (TARGETS MacOSAppSupport DESTINATION lib)
	install (FILES ${MacOSAppSupportHeaderFiles} DESTINATION include)
	install (FILES ${MacOSAppSupportSourceFiles} DESTINATION source)

	# MacOSEmbeddingDemo

	set (MacOSEmbeddingDemoSourcesFolder Sources/MacOSEmbeddingDemo)
	file (GLOB MacOSEmbeddingDemoHeaderFiles ${MacOSEmbeddingDemoSourcesFolder}/*.hpp)
	file (GLOB MacOSEmbeddingDemoSourceFiles
		${MacOSEmbeddingDemoSourcesFolder}/*.cpp
		${MacOSEmbeddingDemoSourcesFolder}/*.mm
	)
	set (
		MacOSEmbeddingDemoFiles
		${MacOSEmbeddingDemoHeaderFiles}
		${MacOSEmbeddingDemoSourceFiles}
	)
	set (
		MacOSEmbeddingDemoResourceFiles
		
	)
	source_group ("Sources" FILES ${MacOSEmbeddingDemoFiles})
	file (GLOB_RECURSE MacOSEmbeddingDemoResourceFiles ${MacOSEmbeddingDemoSourcesFolder}/icons/*.png)
	add_executable (MacOSEmbeddingDemo MACOSX_BUNDLE ${MacOSEmbeddingDemoFiles} ${MacOSEmbeddingDemoResourceFiles})
	set_source_files_properties (${MacOSEmbeddingDemoResourceFiles} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set_target_properties (MacOSEmbeddingDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
	target_include_directories (
		MacOSEmbeddingDemo PUBLIC
		${NodeEngineSourcesFolder}
		${NodeUIEngineSourcesFolder}
		${BuiltInNodesSourcesFolder}
		${MacOSAppSupportSourcesFolder}
	)
	target_link_libraries (MacOSEmbeddingDemo ${CocoaFramework})
	target_link_libraries (MacOSEmbeddingDemo NodeEngine NodeUIEngine BuiltInNodes MacOSAppSupport)
	SetCompilerOptions (MacOSEmbeddingDemo)

endif ()	
