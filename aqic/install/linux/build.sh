clear

function pause(){
   read -p "Appuyer sur une touche pour continuer ..."
}

PRODUCT=aqic
FOLDER=aqic

echo
echo "********************** Build $PRODUCT *************************"

VERSION=1.0.1
QTVER=5.15.2
QTDIR=/home/$USER/Qt/$QTVER/gcc_64
if [ ! -d $DIR ]
then
echo "The folder $QTDIR was not found. Please verify the value of variables QTDIR build from QTVER"
exit 1
fi

DEVEL=/home/$USER/Documents/Development
if [ ! -d $DEVEL ]
then
echo "The folder $DEVEL was not found. Please verify the value of variable DEVEL"
exit 1
fi

OUTPUT=$DEVEL/$FOLDER/$PRODUCT/install/linux/Output
if [ ! -d $OUTPUT ]
then
echo "The folder $OUTPUT was not found. Please verify the value of variables OUTPUT built from FOLDER and PRODUCT."
exit 1
fi

TARGET=$OUTPUT/debian/opt/$PRODUCT
if [ ! -d $OUTPUT/debian/opt ]
then
mkdir $OUTPUT/debian/opt
fi

BINARIES=$DEVEL/build-${FOLDER}-Desktop_Qt_${QTVER//./_}_GCC_64bit-Release
if [ ! -d $BINARIES ]
then
echo "The folder $BINARIES was not found. Please verify the value of variable BINARIES built from DEVEL, FOLDER and QTVER"
exit 1
fi

if [ -d $TARGET ]
then
echo Remove $TARGET
rm -r $TARGET
fi

if [ -f $OUTPUT/$PRODUCT-$VERSION.deb ]
then
echo Remove $OUTPUT/$PRODUCT-$VERSION.deb
chmod +w $OUTPUT/$PRODUCT-$VERSION.deb
rm $OUTPUT/$PRODUCT-$VERSION.deb
fi

if [ -f $OUTPUT/$PRODUCT-$VERSION.zip ]
then
echo Remove $OUTPUT/$PRODUCT-$VERSION.zip
chmod +w $OUTPUT/$PRODUCT-$VERSION.zip
rm $OUTPUT/$PRODUCT-$VERSION.zip
fi

mkdir "$TARGET"

echo Update the control file
sed -i "s/Version: 0.0.0/Version: $VERSION/g" $OUTPUT/debian/DEBIAN/control
sed -i "s/Version=0.0.0/Version=$VERSION/g" $OUTPUT/$PRODUCT.desktop
sed -i "s/Version=0.0.0/Version=$VERSION/g" $OUTPUT/$PRODUCT.run.desktop

echo Copy binary files
cp $BINARIES/$PRODUCT/$PRODUCT $TARGET/$PRODUCT
cp ../../icons/aqic_64.png $TARGET/$PRODUCT.png
cp $PRODUCT-deb.run $TARGET/$PRODUCT.run
chmod +x $TARGET/$PRODUCT.run

echo Copy translation files
mkdir $TARGET/config
cp ../../../languages/*_??.qm $TARGET/config

echo Copy config files
cp -r ../../config $TARGET
rm $TARGET/config/*.geojson

echo Copy .destktop file
cp $OUTPUT/*.desktop $TARGET
chmod 775 $TARGET/*.desktop

echo Copy Qt plugins
mkdir $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqgif.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqtga.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqjpeg.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqwebp.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqicns.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqtiff.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqsvg.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqico.so $TARGET/imageformats
cp $QTDIR/plugins/imageformats/libqwbmp.so $TARGET/imageformats     

mkdir $TARGET/bearer
cp $QTDIR/plugins/bearer/libqgenericbearer.so $TARGET/bearer

mkdir $TARGET/platforms
cp $QTDIR/plugins/platforms/libqxcb.so $TARGET/platforms

mkdir $TARGET/geoservices
cp -r $QTDIR/plugins/geoservices/libqtgeoservices_osm.so $TARGET/geoservices

mkdir $TARGET/platformthemes
cp -r $QTDIR/plugins/platformthemes/libqgtk3.so $TARGET/platformthemes

mkdir $TARGET/position
cp -r $QTDIR/plugins/position/libqtposition_geoclue.so $TARGET/position
cp -r $QTDIR/plugins/position/libqtposition_geoclue2.so $TARGET/position
cp -r $QTDIR/plugins/position/libqtposition_positionpoll.so $TARGET/position
cp -r $QTDIR/plugins/position/libqtposition_serialnmea.so $TARGET/position

mkdir $TARGET/printsupport
cp -r $QTDIR/plugins/printsupport/libcupsprintersupport.so $TARGET/printsupport

mkdir $TARGET/xcbglintegrations
cp -r $QTDIR/plugins/xcbglintegrations/libqxcb-egl-integration.so $TARGET/xcbglintegrations
cp -r $QTDIR/plugins/xcbglintegrations/libqxcb-glx-integration.so $TARGET/xcbglintegrations

echo Copy Qt binary files
mkdir $TARGET/lib
cp $QTDIR/lib/libicudata.so.56 $TARGET/lib
cp $QTDIR/lib/libicui18n.so.56 $TARGET/lib
cp $QTDIR/lib/libicuuc.so.56 $TARGET/lib
cp $QTDIR/lib/libQt53DAnimation.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DCore.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DExtras.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DInput.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DLogic.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DQuickAnimation.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DQuickExtras.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DQuickInput.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DQuickRender.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DQuickScene2D.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DQuick.so.5 $TARGET/lib
cp $QTDIR/lib/libQt53DRender.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Charts.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Concurrent.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Core.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5DBus.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Gui.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Location.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Network.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5OpenGL.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5PositioningQuick.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Positioning.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5PrintSupport.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Qml.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5QuickControls2.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5QuickParticles.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5QuickShapes.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Quick.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5QuickTemplates2.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5QuickWidgets.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Svg.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Widgets.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5XcbQpa.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5Gamepad.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5SerialPort.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5VirtualKeyboard.so.5 $TARGET/lib
cp $QTDIR/lib/libQt5QmlWorkerScript.so.5.15.2 $TARGET/lib/libQt5QmlWorkerScript.so.5
cp $QTDIR/lib/libQt5QmlModels.so.5.15.2 $TARGET/lib/libQt5QmlModels.so.5

echo Copy Qt qml
cp    $QTDIR/qml/builtins.qmltypes $TARGET
cp -r $QTDIR/qml/QtLocation $TARGET
cp -r $QTDIR/qml/QtPositioning $TARGET
cp -r $QTDIR/qml/QtQuick $TARGET
cp -r $QTDIR/qml/QtQuick.2 $TARGET
cp -r $QTDIR/qml/QtGraphicalEffects $TARGET
cp -r $QTDIR/qml/Qt $TARGET

echo Remove unused files
CURRENTFOLDER=$PWD
cd $TARGET
find -path "./*.so.debug" -delete
cd $CURRENTFOLDER

echo Copy Qt translation files.
mkdir $TARGET/qt_translations
cp $QTDIR/translations/qt_*.* $TARGET/qt_translations
cp $QTDIR/translations/qtbase_*.* $TARGET/qt_translations
cp $QTDIR/translations/qtmultimedia_*.* $TARGET/qt_translations
cp $QTDIR/translations/qtscript_*.* $TARGET/qt_translations
cp $QTDIR/translations/qtxmlpatterns_*.* $TARGET/qt_translations

echo Copy ssl file
cp ssl/libssl.so.1.1 $TARGET/lib
cp ssl/libcrypto.so.1.1 $TARGET/lib
chmod -R a+r $TARGET

#********************************************************************
echo Build debian package
cd Output
chmod 755 debian/DEBIAN/
chmod 755 debian/DEBIAN/*
dpkg-deb --verbose --build debian $PRODUCT-linux64-x86-$VERSION.deb
echo "-----------------> $PRODUCT-linux64-x86-$VERSION.deb generated"
cd ..
#********************************************************************

#********************************************************************
echo Build spec file for rpm installer
# PLATEFORM is the name of rpm file.
PLATEFORM=$PRODUCT-$VERSION-1.fc33.x86_64
SPECS=rpmbuild/SPECS/$PRODUCT.spec
if [ -d rpmbuild/BUILDROOT/$PRODUCT* ]
then
rm -r rpmbuild/BUILDROOT/$PRODUCT*
fi
mkdir rpmbuild/BUILDROOT/$PLATEFORM
mkdir rpmbuild/BUILDROOT/$PLATEFORM/opt
cp -R $OUTPUT/debian/opt/$PRODUCT rpmbuild/BUILDROOT/$PLATEFORM/opt/
# Conflict Qt 5.15.2 and Fedora 33 about lib ssl. Remove it
rm rpmbuild/BUILDROOT/$PLATEFORM/opt/aqic/lib/libcrypto.so.1.1
rm rpmbuild/BUILDROOT/$PLATEFORM/opt/aqic/lib/libssl.so.1.1
#
cp rpmbuild/SPECS/$PRODUCT-rpm.spec rpmbuild/SPECS/$PRODUCT.spec
sed -i "s/Version:        0.0.0/Version:        $VERSION/g" rpmbuild/SPECS/$PRODUCT.spec
find rpmbuild/BUILDROOT/$PLATEFORM/opt/$PRODUCT -iname '*' -type d >> $SPECS
sed -i "s|rpmbuild/BUILDROOT/$PLATEFORM/opt|%dir /opt|g" $SPECS
find rpmbuild/BUILDROOT/$PLATEFORM/opt/$PRODUCT -iname '*' -type f >> $SPECS
sed -i "s|rpmbuild/BUILDROOT/$PLATEFORM/opt|/opt|g" $SPECS
echo Create the shared folder rpmbuild
# SHAREDFOLDER is a folder shared by Debian and REDHART linux to build rpm file.
SHAREDFOLDER=/media/sf_temp
if [ -d $SHAREDFOLDER/rpmbuild ]
then
rm -r $SHAREDFOLDER/rpmbuild
fi
cp -r rpmbuild $SHAREDFOLDER
rm -r $SHAREDFOLDER/rpmbuild/SPECS/$PRODUCT-rpm.spec
rm rpmbuild/SPECS/$PRODUCT.spec
echo "-----------------> $PRODUCT-linux64-x86-$VERSION.spec generated"
#********************************************************************

#********************************************************************
#echo Build Qt installer
#sed -i "s|$PRODUCT/|$PRODUCT/$PRODUCT/|g" $OUTPUT/debian/opt/aqic/aqic.run
#XMLCONFIG=$OUTPUT/../Qt-Installer/config/config.xml
#XMLPACKAGE=$OUTPUT/../Qt-Installer/packages/com.vendor.product/meta/package.xml
#cd $OUTPUT/../Qt-Installer
#sed -i "s/<Version>0.0.0/<Version>$VERSION/g" $XMLCONFIG
#sed -i "s/<Version>0.0.0/<Version>$VERSION/g" $XMLPACKAGE
#NOW=$(date +"%Y-%m-%d")
#sed -i "s|<ReleaseDate></ReleaseDate>|<ReleaseDate>$NOW</ReleaseDate>|g" $XMLPACKAGE
#$QTDIR/bin/lrelease ../fr.ts
#mv ../fr.qm packages/com.vendor.product/meta
#7z a -mx9 $OUTPUT/../Qt-Installer/packages/com.vendor.product/data/aqic.7z $OUTPUT/debian/opt/aqic
#$QTDIR/../../Tools/QtInstallerFramework/4.0/bin/binarycreator -c config/config.xml -p packages $OUTPUT/$PRODUCT-linux64-x86-$VERSION.run
#cd ..
#sed -i "s/<Version>$VERSION/<Version>0.0.0/g" $XMLCONFIG
#sed -i "s/<Version>$VERSION/<Version>0.0.0/g" $XMLPACKAGE
#sed -i "s|<ReleaseDate>$NOW</ReleaseDate>|<ReleaseDate></ReleaseDate>|g" $XMLPACKAGE
#sed -i "s|$PRODUCT/$PRODUCT/|$PRODUCT/|g" $OUTPUT/debian/opt/aqic/aqic.run
#echo "-----------------> $PRODUCT-linux64-x86-$VERSION.run generated"
#********************************************************************

#********************************************************************
echo Build zip file
cp $PRODUCT-zip.run $TARGET/$PRODUCT.run
chmod +x $TARGET/$PRODUCT.run
cd $TARGET
cd ..
zip -v -q -r -9 $OUTPUT/$PRODUCT-linux64-x86-$VERSION.zip $PRODUCT
echo "-----------------> $PRODUCT-linux64-x86-$VERSION.zip generated"
#********************************************************************

echo Set the version at 0.0.0
sed -i "s/Version: $VERSION/Version: 0.0.0/g" $OUTPUT/debian/DEBIAN/control
sed -i "s/Version=$VERSION/Version=0.0.0/g" $OUTPUT/$PRODUCT.desktop
sed -i "s/Version=$VERSION/Version=0.0.0/g" $OUTPUT/$PRODUCT.run.desktop
rm -d -r $TARGET

echo "********************** End build *************************"

