#!/bin/bash

DELIM_1="--------------------------------------------------------------------------------"
DELIM_2="================================================================================"

Msg()
{
	echo ${DELIM_1}
	echo -e ">> $@"
}

Fail()
{
	echo ${DELIM_2}
	echo -e "ERROR: $@"
	echo ${DELIM_2}
	kill -SIGINT $$
	exit 1
}

Try() { "$@" || Fail "$@ Failed!"; }

CheckResult() { test $? = 0 || Fail "Build failed"; }

CheckVar() { test -n "$(echo $1)" || Fail "$1 environment variable is not set! $2"; }

CheckCmd() { which $1 > /dev/null; test $? = 0 || Fail "You have no [$1] command. Install '$2' package"; }

CheckHeader() { test "$(find /usr/include /usr/local/include -name "$1" 2>/dev/null | wc -l)" != 0 || Fail "You have no $1 header. Install '$2' package"; }

CheckPythonModule() { python -c "import $1" 2>/dev/null; test $? = 0 || Fail "There is no '$1' python module installed"; }

FilterPath() { echo $1 | sed "s/$(echo $2 | sed "s/\//\\\\\//g")//g" | sed "s/::/:/g" | sed "s/^://" | sed "s/:$//"; }

AddToPath() { export PATH="$1:$(FilterPath "$PATH" "$1")"; }

AddToLibPath() { export LD_LIBRARY_PATH="$1:$(FilterPath "$LD_LIBRARY_PATH" "$1")"; }

EnableFlag()
{
	local __FLAGS=${!1}
	__FLAGS=$(echo ${__FLAGS} | sed "s/$2//g")
	__FLAGS+=" $2"
	eval export $1=\${__FLAGS}
}

DisableFlag()
{
	local __FLAGS=${!1}
	__FLAGS=$(echo ${__FLAGS} | sed "s/$2//g")
	eval export $1=\${__FLAGS}
}

SetFlag()
{
	test "$3" = disable && DisableFlag $1 $2 || EnableFlag $1 $2
}

AskFor()
{
	test -n "$@" || return

	local Question=$(echo "$@" | sed -r "s/^(.*)\[[^\[]*\]$/\1/")
	local DefaultAnswer=$(echo "$@" | grep "^.*\[[^\[]*\]*$" | sed -r "s/^.*\[([^\[]*)\]$/\1/")

	read -p "$@: "
	test $? != 0 -o -z "${REPLY}" && Answer=${DefaultAnswer} || Answer=${REPLY}
	export ANSWER=${Answer}
}

AskToConfirm()
{
	local Message="$1"
	local RequiredAnswer="$2"
	AskFor "$Message"
	local Answer="$(echo $ANSWER | awk '{print tolower($0)}')"
	test "$Answer" = "$RequiredAnswer"
}

AskToConfirmWithYes()
{
	local Message="$@"
	AskToConfirm "$Message" "yes"
}
