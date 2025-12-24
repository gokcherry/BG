cmake_minimum_required(VERSION 3.5)

include(CMakePrintHelpers)  #cmake_print_variable   fonksiyonu için
#proje ismi ayarlanıyor.
string(FIND ${CMAKE_CURRENT_SOURCE_DIR} / BaslangicIndeks REVERSE)
math(EXPR BaslangicIndeks "${BaslangicIndeks}+1")
string(SUBSTRING ${CMAKE_CURRENT_SOURCE_DIR} ${BaslangicIndeks} -1 ProjeIsmi)

#proje ismi belirleniyor
project(${ProjeIsmi})

#proje klasörünün yolu yeni bir değişkene atanıyor
set(Project_Dir ${CMAKE_SOURCE_DIR})

set(Kutuphane_Dir ${Project_Dir}/../../Kutuphaneler)

#inlcude ve src klasör yollarıda yeni bir değişkene atanıyor
set(Project_Include_Dir ${Project_Dir}/include)

set(Project_Src_Dir ${Project_Dir}/src)

set(ImGui_Source_files  ${Kutuphane_Dir}/imgui/imgui.cpp
                        ${Kutuphane_Dir}/imgui/imgui_draw.cpp
                        ${Kutuphane_Dir}/imgui/imgui_tables.cpp
                        ${Kutuphane_Dir}/imgui/imgui_widgets.cpp
                        ${Kutuphane_Dir}/imgui/backends/imgui_impl_glfw.cpp
                        ${Kutuphane_Dir}/imgui/backends/imgui_impl_opengl3.cpp
                        )


#Kaynak Dosyalari ekleniyor
set(Project_Source_Files 
            ${Project_Src_Dir}/main.cpp
            ${Project_Src_Dir}/glad.c
            ${Project_Src_Dir}/glwindow.cpp
            ${Project_Src_Dir}/shaderfile.cpp
            ${Project_Src_Dir}/shaderprogram.cpp
            ${Project_Src_Dir}/vertexbuffer.cpp
            ${Project_Src_Dir}/vertexarrayobject.cpp
            ${Project_Src_Dir}/meshloader.cpp        
            ${Project_Src_Dir}/texturemanager.cpp  
            ${Project_Src_Dir}/transform.cpp 
            ${Project_Src_Dir}/indexbuffer.cpp         
            ${Project_Src_Dir}/shadermanager.cpp  
            ${Project_Src_Dir}/sceneobject.cpp                    
            ${Project_Src_Dir}/model.cpp         
            ${Project_Src_Dir}/camera.cpp     
            ${Project_Src_Dir}/viewport.cpp             
            ${Project_Src_Dir}/scene.cpp                             
            ${ImGui_Source_files}    
    )

include_directories(
    ${Project_Include_Dir}
    ${Kutuphane_Dir}
    ${Kutuphane_Dir}/imgui
    )




set(glfw3_DIR ${Kutuphane_Dir}/glfw/lib/cmake/glfw3/)

find_package(glfw3 3.4 REQUIRED)

add_executable(${PROJECT_NAME} ${Project_Source_Files})

target_link_libraries(${PROJECT_NAME} glfw)