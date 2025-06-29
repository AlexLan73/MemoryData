<?xml version="1.0" encoding="utf-8"?> 
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
<!--
<ImportGroup Label="MemoryData" />
  <PropertyGroup Label="UserMacros" />
-->

  <PropertyGroup >
    <!-- Определения препроцессора, если нужны специфичные для проекта -->
    <PreprocessorDefinitions>_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
  </PropertyGroup>


  <ItemDefinitionGroup>
    <ClCompile>
      <!-- Пути к заголовочным файлам -->
      <!-- 1. Пути к заголовочным файлам Boost и msgpack.hpp -->

      <!-- Предполагается, что эти файлы находятся в папке 'MemoryData' относительно папки с проектом -->
      <!-- Или в папке, где находится сам .props файл, если он расположен в корне решения -->
      <AdditionalIncludeDirectories>
        F:\C++\Boost\boost_1_87_0\;
        F:\C++\Boost\vcpkg\installed\x64-windows\include\;
        <!-- 3. Дополнительные каталоги включения (проект)\MemoryData\include; (проект) -->
        $(ProjectDir)\include;
        $(ProjectDir);
        %(AdditionalIncludeDirectories)
      </AdditionalIncludeDirectories>
      <!-- Если MessagePack установлен в стандартное место или доступен через относительный путь -->
      <!-- <AdditionalIncludeDirectories>$(SolutionDir)path\to\msgpack\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories> -->

      <!-- Уровень предупреждений -->
      <WarningLevel>Level3</WarningLevel>
      <!-- Отключение определенных предупреждений, если это необходимо -->
      <!-- <DisableSpecificWarnings>4005;%(DisableSpecificWarnings)</DisableSpecificWarnings> -->

      <!-- Убедитесь, что у вас задан стандарт C++20, как в vcxproj -->
      <LanguageStandard>stdcpp20</LanguageStandard>

    </ClCompile>
    <Link>
      <!-- Дополнительные зависимости библиотек -->
      <!-- Если используются специфические библиотеки, например, для MessagePack или других зависимостей -->
      <!-- <AdditionalDependencies>some_library.lib;%(AdditionalDependencies)</AdditionalDependencies> -->
      <!-- Пути к библиотекам -->
      <!-- 2. Каталоги LIB библиотек -->
      <!-- <AdditionalLibraryDirectories>
        $(SolutionDir)path\to\msgpack\lib;
        %(AdditionalLibraryDirectories)
        </AdditionalLibraryDirectories> -->

      <AdditionalLibraryDirectories>
        <!-- Общие пути для Boost (обычно Stage\Lib содержит и debug, и release версии) -->
        F:\C++\Boost\boost_1_87_0\stage\lib;
        <!-- Пути для msgpack (разные для Debug и Release) -->
        <!-- Обрати внимание на условие, которое проверяет конфигурацию сборки -->
        $([System.String]::Copy('%(AdditionalLibraryDirectories)').Contains('debug')) ? 'F:\C++\Boost\vcpkg\installed\x64-windows\debug\lib' : 'F:\C++\Boost\vcpkg\installed\x64-windows\lib';
        %(AdditionalLibraryDirectories)
      </AdditionalLibraryDirectories>
      <!-- Добавьте сюда специфичные для проекта .lib файлы, если они есть -->
      <!-- <AdditionalDependencies>
        some_boost_library.lib;
        some_msgpack_library.lib;
        %(AdditionalDependencies)
      </AdditionalDependencies> -->

    </Link>
  </ItemDefinitionGroup>

  <ItemGroup />
</Project>



