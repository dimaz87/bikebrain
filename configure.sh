#!/bin/bash

test "$0" == "${BASH_SOURCE}" || { echo "Couldn't be sourced"; return 1; }
RootDirectory="$(dirname "$0")"
RootDirectory="$(realpath "${RootDirectory}")"
source "${RootDirectory}/header.sh"

Requirements=(gdebi libncurses5:i386 libstdc++6:i386)

MissedRequirements=()
for r in "${Requirements[@]}"; do
	dpkg -s $r > /dev/null 2>&1
	test $? = 0 || MissedRequirements+=($r)
done
test -z "${MissedRequirements[@]}" || { echo -e "First install required packages:\n\tsudo apt-get install ${MissedRequirements[@]}"; exit 1; }

##############################
###### INSTALL HOST SDK ######

EdisonSdkArchive=edison-sdk-linux64-ww18-15.zip
EdisonSdkInstallFile=poky-edison-eglibc-x86_64-edison-image-core2-32-toolchain-1.6.1.sh
EdisonSdkInstallPath="${RootDirectory}/toolchain/edison-sdk-linux64-ww18-15"

DoInstall()
{
	local installScript=$1
	local installPath=$2
	Try ./${installScript} << ANSWERS
	${installPath}
	Y
ANSWERS
}

if test ! -d "${EdisonSdkInstallPath}"; then
	(
		cd "${RootDirectory}"
		mkdir src 2>/dev/null
		cd src
		test ! -r ${EdisonSdkArchive} -a ! -r ${EdisonSdkInstallFile} && Try wget "http://downloadmirror.intel.com/24910/eng/${EdisonSdkArchive}"
		test ! -r ${EdisonSdkInstallFile} && Try unzip -x edison-sdk-linux64-ww18-15.zip
		Try DoInstall poky-edison-eglibc-x86_64-edison-image-core2-32-toolchain-1.6.1.sh "${EdisonSdkInstallPath}"
	)
fi

#############################
###### INSTALL MCU SDK ######

McuSdkArchive=edison-mcusdk-linux64-1.0.10.zip
McuSdkInstallPath="${RootDirectory}/toolchain/edison-mcusdk-linux64-1.0.10"

if test ! -d "${McuSdkInstallPath}"; then
	(
		cd "${RootDirectory}"
		mkdir src 2>/dev/null
		cd src
		test ! -r ${McuSdkArchive} && Try wget "http://downloadmirror.intel.com/24910/eng/${McuSdkArchive}"
		Try unzip -x ${McuSdkArchive} -d "${McuSdkInstallPath}"
	)
fi

#########################
###### SETUP CMAKE ######

BuildArtifactsToDelete="CMakeCache.txt CMakeFiles Makefile
	ui.argile/CMakeCache.txt ui.argile/CMakeFiles ui.argile/Makefile
	ui.shell/CMakeCache.txt ui.shell/CMakeFiles ui.shell/Makefile
	stingray_modules/CMakeFiles stingray_modules/CMakeCache.txt stingray_modules/Makefile
	stingray/stingraykit/CMakeCache.txt stingray/stingraykit/CMakeFiles stingray/stingraykit/Makefile stingray/stingraykit/lib
	build.ninja rules.ninja .ninja_deps .ninja_logs"

( cd ${RootDirectory}; rm -rf ${BuildArtifactsToDelete}; )

rm -rf ${BuildArtifactsToDelete}

cmake -DTOOLCHAIN_PATH:FILEPATH="${EdisonSdkInstallPath}" -DTOOLCHAIN_FILE="${RootDirectory}/edison.cmake" "${RootDirectory}" >/dev/null 2>&1
cmake -DTOOLCHAIN_PATH:FILEPATH="${EdisonSdkInstallPath}" -DTOOLCHAIN_FILE="${RootDirectory}/edison.cmake" "${RootDirectory}"
