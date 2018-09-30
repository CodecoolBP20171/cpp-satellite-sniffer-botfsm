@ECHO OFF

SET solDir=%1
SET targetDir=%2

REM Copy all resource files to build directory
COPY /Y %solDir%resources\fonts\* %targetDir%*
COPY /Y %solDir%resources\settings\* %targetDir%*
COPY /Y %solDir%resources\textures\* %targetDir%*
MD %targetDir%satellites
COPY /Y %solDir%resources\satellite_data\satellites\* %targetDir%satellites\*

REM Copy all depedency librarys to build directory
COPY /Y %solDir%libs\*.dll %targetDir%*

EXIT 0
