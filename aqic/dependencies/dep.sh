echo "Copy dependencies"
if [ ! -d "$2/config" ]
then
mkdir $2/config
fi
cp $1/../languages/*.qm $2/config
cp -R $1/config $2/
# rm $2/config/*.geojson

