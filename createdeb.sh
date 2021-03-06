#!/bin/bash

base=$PWD

#Make sure we dont have any old debs here...
rm -f *.deb

# First part:
# collect data and create a control file.

pushd . || exit 2

EXEC="M2W_Sensor"

APP="m2wsensor"
#Or some git info...
VER=$(date +'%Y%m%d%H%M')
NUM="1"
ARCH=$(dpkg --print-architecture)

USER=$(git config user.name)
MAIL=$(git config user.email)

NAME=$APP"_"$VER"-"$NUM"_"$ARCH

mkdir -p $base/$NAME/DEBIAN || exit 10
control=$base/$NAME/DEBIAN/control

echo "Package: "$APP        >  $control
echo "Version: "$VER"-"$NUM >> $control
echo "Section: base"        >> $control
echo "Priority: optional"   >> $control
echo "Architecture: "$ARCH  >> $control

echo "Depends: "            >>  $control
echo "Maintainer: "$USER" <"$MAIL">" >>  $control

cat >> $control << EOF
Description: M2W_Sensor
  Exprimental debian package
EOF

#Misc git info...
## echo "  git describe: "$(git describe --tags)     >> $control
echo "  git log: "$(git log --oneline | head -n1) >> $control

qmake   || exit 20
make  || exit 22
#copy skeleton

(cd debian/; tar c . )|(cd $NAME ; tar x)

install -d $NAME/usr/sbin || exit 30
cp $EXEC $NAME/usr/sbin/   || exit 32

# And the create the package
popd
dpkg-deb --build $NAME || exit 40

rm -rf $NAME/ || exit 50

echo "Done..."
exit 0
