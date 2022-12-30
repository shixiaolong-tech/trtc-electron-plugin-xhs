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

echo "--------------------------------------copy beauty resources"
mkdir dist-temp\face_beauty.xhs
mkdir dist-temp\face_beauty.xhs\Beauty_Res
mkdir dist-temp\face_beauty.xhs\Lut_Res
mkdir dist-temp\face_beauty.xhs\vsco1
mkdir dist-temp\face_beauty.xhs\vsco2
mkdir dist-temp\face_beauty.xhs\vsco3
xcopy /s ..\.resources\beautify\face_beauty.xhs\Beauty_Res\* dist-temp\face_beauty.xhs\Beauty_Res\*
xcopy /s ..\.resources\beautify\face_beauty.xhs\Lut_Res\* dist-temp\face_beauty.xhs\Lut_Res\*
xcopy /s ..\.resources\beautify\face_beauty.xhs\vsco1\* dist-temp\face_beauty.xhs\vsco1\*
xcopy /s ..\.resources\beautify\face_beauty.xhs\vsco2\* dist-temp\face_beauty.xhs\vsco2\*
xcopy /s ..\.resources\beautify\face_beauty.xhs\vsco3\* dist-temp\face_beauty.xhs\vsco3\*

cd dist-temp 
dir
7z a -tzip ../release-win64.zip ./*
cd ..
echo "--------------------------------------zip end"
