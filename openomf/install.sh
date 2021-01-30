#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2020-present Cebion

INSTALLPATH="/storage/roms/ports"
PKG_NAME="openomf"
PKG_FILE="openomf-gamedata.zip"
PKG_VERSION="1.5"
PKG_SHASUM="690923ed254bb1ee85d7d5e0e152e69aaccdbab87df9607d2439c08cef676591"
SOURCEPATH=$(pwd)
### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}/${PKG_NAME}

curl -Lo ${PKG_FILE} https://github.com/Cebion/packages/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}

### Create the start script
cat <<EOF >${INSTALLPATH}/"Openomf.sh"
#!/bin/bash
source /etc/profile
/usr/bin/show_splash.sh "OpenOMF"
clear >/dev/console

export LD_LIBRARY_PATH=${INSTALLPATH}/${PKG_NAME}/lib:/usr/lib
cd ${INSTALLPATH}/${PKG_NAME}
  ./bin/openomf

EOF

### Add images if not exist folder
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

### Add videos if not exist folder
if [ ! -d "${INSTALLPATH}/videos" ]
then
  mkdir -p "${INSTALLPATH}/videos"
fi

for image in system-openomf-thumb.png system-openomf.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

for video in system-openomf.mp4
do
  cp "${SOURCEPATH}/${PKG_NAME}/${video}" "${INSTALLPATH}/videos"
done

### Add Game to the game list XML
if [ ! "$(grep -q 'OpenOMF' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Openomf.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'OpenOMF'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'OpenOMF is an Open Source remake of "One Must Fall 2097"'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-openomf.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-openomf-thumb.png'\
                -s '//gameList/game[last()]' -t elem -n 'video'       -v './videos/system-openomf.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '4.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '1994'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Diversions Entertainment'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'Moon Doggie'\
		${INSTALLPATH}/gamelist.xml
fi
