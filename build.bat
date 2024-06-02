@echo off

if not exist Build ( mkdir Build )
if not exist Build\Debug ( mkdir Build\Debug )
if not exist Build\Release ( mkdir Build\Release )

set "BinaryTreeClass=Source\Structure\PriorityQueue.cpp"
set "PriorityQueueClass=Source\Structure\PriorityQueue.cpp"
set "StructureID=-ISource\Structure"

set "AllClasses=%PriorityQueueClass% %BinaryTreeClass%"

set "IncludeDirectories=-ISource\Lib %StructureID%"
set "MainFileDirectory=Source\Main.cpp"

set "BuildType=Debug"

set "OutputDirectory=Build\%BuildType%"

set "OutputName=HuffmanCoding"

g++ -o %OutputDirectory%/%OutputName% %MainFileDirectory% %AllClasses% -lgdi32 %IncludeDirectories%

"%OutputDirectory%\%OutputName%.exe"