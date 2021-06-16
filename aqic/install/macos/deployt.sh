echo "************ Start MacOS package creation *************"

VERSION=1.0.1
PRODUCT=aqic
FOLDER=aqic
QTDIR=/Users/$USER/Qt5152/5.15.2/clang_64
DEVEL=/Users/$USER/Documents/Development/$FOLDER/$PRODUCT
BINARIES=/Users/$USER/Documents/Development/build-aqic-Desktop_Qt_5_15_2_clang_64bit-Release

if [ -d Output ]
then
echo Remove Output
rm -r Output
fi

mkdir Output

WD=$PWD
cd $BINARIES/$PRODUCT

echo "launch macdeployqt"
$QTDIR/bin/macdeployqt $PRODUCT.app -verbose=1 -dmg -qmldir=$DEVEL -libpath=$DYLIB -always-overwrite
echo "Move dmg file"
cd $WD
if [ -f Output/$PRODUCT-$VERSION.dmg ]
then
rm Output/$PRODUCT-$VERSION.dmg
fi
mv $BINARIES/$PRODUCT/$PRODUCT.dmg Output/
mv Output/$PRODUCT.dmg Output/$PRODUCT-macos64-$VERSION.dmg

echo "***************** Finished ******************"



