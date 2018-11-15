cmake_minimum_required (VERSION 3.6)

function (SetCompilerOptions module maxWarningLevel)
	set (DebugOptions)
	set (ReleaseOptions)
	if (MSVC)
		set (DebugOptions -DDEBUG -D_DEBUG /ZI /JMC /sdl /GR)
		set (ReleaseOptions /Oi /Gy)
		if (${maxWarningLevel})
			target_compile_options (${module} PRIVATE /W4 /WX)
		endif ()
	else ()
		set (DebugOptions -DDEBUG -D_DEBUG -g -ggdb)
		if (${maxWarningLevel})
			target_compile_options (${module} PRIVATE -Wall -Wextra -Werror)
		endif ()
		target_compile_options (${module} PRIVATE -std=c++11 -stdlib=libc++)
	endif ()
	target_compile_options(${module} PUBLIC "$<$<CONFIG:DEBUG>:${DebugOptions}>")
	target_compile_options(${module} PUBLIC "$<$<CONFIG:RELEASE>:${ReleaseOptions}>")
endfunction ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release)
set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/DevKit CACHE PATH "Install prefix.")
set (WXWIDGETS_DIR $ENV{WXWIDGETS_DIR} CACHE STRING "wxWidgets binary directory.")

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
target_include_directories (NodeEngine PUBLIC ${NodeEngineSourcesFolder})
SetCompilerOptions (NodeEngine ON)
install (TARGETS NodeEngine DESTINATION lib_debug CONFIGURATIONS Debug)
install (TARGETS NodeEngine DESTINATION lib_release CONFIGURATIONS Release)
install (FILES ${NodeEngineHeaderFiles} DESTINATION include)

# NodeUIEngine

set (NodeUIEngineSourcesFolder Sources/NodeUIEngine)
file (GLOB NodeUIEngineHeaderFiles ${NodeUIEngineSourcesFolder}/*.hpp)
file (GLOB NodeUIEngineSourceFiles ${NodeUIEngineSourcesFolder}/*.cpp)
set (
	NodeUIEngineFiles
	${NodeUIEngineHeaderFiles}
	${NodeUIEngineSourceFiles}
)
source_group ("Sources" FILES ${NodeUIEngineFiles})
add_library (NodeUIEngine STATIC ${NodeUIEngineFiles})
target_include_directories (NodeUIEngine PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder})
target_link_libraries (NodeUIEngine NodeEngine)
SetCompilerOptions (NodeUIEngine ON)
install (TARGETS NodeUIEngine DESTINATION lib_debug CONFIGURATIONS Debug)
install (TARGETS NodeUIEngine DESTINATION lib_release CONFIGURATIONS Release)
install (FILES ${NodeUIEngineHeaderFiles} DESTINATION include)

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
target_include_directories (BuiltInNodes PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder} ${BuiltInNodesSourcesFolder})
target_link_libraries (BuiltInNodes NodeEngine NodeUIEngine)
SetCompilerOptions (BuiltInNodes ON)
install (TARGETS BuiltInNodes DESTINATION lib_debug CONFIGURATIONS Debug)
install (TARGETS BuiltInNodes DESTINATION lib_release CONFIGURATIONS Release)
install (FILES ${BuiltInNodesHeaderFiles} DESTINATION include)

# NodeEngineTest

set (TestFrameworkSourcesFolder Sources/TestFramework)
set (NodeEngineTestSourcesFolder Sources/NodeEngineTest)
file (GLOB TestFrameworkFiles ${TestFrameworkSourcesFolder}/*.hpp ${TestFrameworkSourcesFolder}/*.cpp)
file (GLOB NodeEngineTestHeaderFiles ${NodeEngineTestSourcesFolder}/*.hpp)
file (GLOB NodeEngineTestSourceFiles ${NodeEngineTestSourcesFolder}/*.cpp)
set (
	NodeEngineTestFiles
	${TestFrameworkFiles}
	${NodeEngineTestHeaderFiles}
	${NodeEngineTestSourceFiles}
)
source_group ("Sources" FILES ${NodeEngineTestFiles})
add_executable (NodeEngineTest ${NodeEngineTestFiles})
target_include_directories (NodeEngineTest PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder} ${BuiltInNodesSourcesFolder} ${TestFrameworkSourcesFolder})
target_link_libraries (NodeEngineTest NodeEngine NodeUIEngine BuiltInNodes)
get_filename_component(NodeEngineTestSourcesFolderAbsolute "${CMAKE_CURRENT_LIST_DIR}/${NodeEngineTestSourcesFolder}" ABSOLUTE)
add_custom_command (TARGET NodeEngineTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${NodeEngineTestSourcesFolderAbsolute}/VisualTestFiles $<TARGET_FILE_DIR:NodeEngineTest>/VisualTestFiles
)
SetCompilerOptions (NodeEngineTest ON)
add_test (NodeEngineTest NodeEngineTest)

if (MSVC)

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
	target_include_directories (WindowsAppSupport PUBLIC ${NodeEngineSourcesFolder} ${NodeUIEngineSourcesFolder} ${BuiltInNodesSourcesFolder} ${WindowsAppSupportSourcesFolder})
	target_link_libraries (WindowsAppSupport NodeEngine NodeUIEngine BuiltInNodes)
	SetCompilerOptions (WindowsAppSupport OFF)
	install (TARGETS WindowsAppSupport DESTINATION lib_debug CONFIGURATIONS Debug)
	install (TARGETS WindowsAppSupport DESTINATION lib_release CONFIGURATIONS Release)
	install (FILES ${WindowsAppSupportHeaderFiles} DESTINATION include)

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
	target_include_directories (
		WindowsEmbeddingDemo PUBLIC
		${NodeEngineSourcesFolder}
		${NodeUIEngineSourcesFolder}
		${BuiltInNodesSourcesFolder}
		${TestFrameworkSourcesFolder}
		${WindowsAppSupportSourcesFolder}
	)
	target_link_libraries (WindowsEmbeddingDemo NodeEngine NodeUIEngine BuiltInNodes WindowsAppSupport)
	SetCompilerOptions (WindowsEmbeddingDemo OFF)
	
	if (NOT ${WXWIDGETS_DIR} STREQUAL "")
		link_directories (${WXWIDGETS_DIR}/lib/vc_x64_lib)

		# wxWidgetsAppSupport
		
		set (wxWidgetsAppSupportSourcesFolder Sources/wxWidgetsAppSupport)
		file (GLOB wxWidgetsAppSupportHeaderFiles ${wxWidgetsAppSupportSourcesFolder}/*.hpp)
		file (GLOB wxWidgetsAppSupportSourceFiles ${wxWidgetsAppSupportSourcesFolder}/*.cpp)
		set (
			wxWidgetsAppSupportFiles
			${wxWidgetsAppSupportHeaderFiles}
			${wxWidgetsAppSupportSourceFiles}
		)
		source_group ("Sources" FILES ${wxWidgetsAppSupportFiles})
		add_library (wxWidgetsAppSupport STATIC ${wxWidgetsAppSupportFiles})
		target_compile_definitions (wxWidgetsAppSupport PUBLIC _CRT_SECURE_NO_WARNINGS)
		target_include_directories (
			wxWidgetsAppSupport PUBLIC
			${wxWidgetsAppSupport}
			${NodeUIEngineSourcesFolder}
			${BuiltInNodesSourcesFolder}
			${WindowsAppSupportSourcesFolder}
			${WXWIDGETS_DIR}/include
			${WXWIDGETS_DIR}/include/msvc
		)
		target_link_libraries (wxWidgetsAppSupport NodeEngine NodeUIEngine BuiltInNodes)
		SetCompilerOptions (wxWidgetsAppSupport OFF)
		install (TARGETS wxWidgetsAppSupport DESTINATION lib_debug CONFIGURATIONS Debug)
		install (TARGETS wxWidgetsAppSupport DESTINATION lib_release CONFIGURATIONS Release)
		install (FILES ${wxWidgetsAppSupportHeaderFiles} DESTINATION include)
		
		# WindowsTestApp

		set (WindowsTestAppSourcesFolder Sources/WindowsTestApp)
		file (GLOB WindowsTestAppHeaderFiles ${WindowsTestAppSourcesFolder}/*.hpp)
		file (GLOB WindowsTestAppSourceFiles ${WindowsTestAppSourcesFolder}/*.cpp)
		set (
			WindowsTestAppFiles
			${WindowsTestAppHeaderFiles}
			${WindowsTestAppSourceFiles}
		)
		source_group ("Sources" FILES ${WindowsTestAppFiles})
		add_executable (WindowsTestApp WIN32 ${WindowsTestAppFiles})
		target_include_directories (
			WindowsTestApp PUBLIC
			${NodeEngineSourcesFolder}
			${NodeUIEngineSourcesFolder}
			${BuiltInNodesSourcesFolder}
			${TestFrameworkSourcesFolder}
			${WindowsAppSupportSourcesFolder}
			${wxWidgetsAppSupportSourcesFolder}
			${WXWIDGETS_DIR}/include
			${WXWIDGETS_DIR}/include/msvc		
		)
		target_link_libraries (WindowsTestApp NodeEngine NodeUIEngine BuiltInNodes WindowsAppSupport wxWidgetsAppSupport)
		SetCompilerOptions (WindowsTestApp OFF)
	
	endif ()
	
endif ()
