@echo off

rmdir /s/q x64
msbuild TRTCElectronBeautyPlugin.sln /p:configuration="Release" /p:platform="x64"
if %errorlevel% NEQ 0 (
  echo "--------------------------------------msbuild error!"
  goto failed
)

echo "--------------------------------------copy lib binary"
xcopy ..\XHS-beauty-lib\win\win64\*.* .\x64\Release\ /s/y
echo "--------------------------------------build success!"

echo "--------------------------------------zip build"
rmdir /s/q dist-temp 
del release-win64.zip

mkdir dist-temp
copy /Y x64\Release\* dist-temp\*
cd dist-temp 
dir
7z a -tzip ../release-win64.zip ./*
cd ..
echo "--------------------------------------zip end"
