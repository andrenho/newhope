#!/bin/sh

if [ $# -eq 1 ]; then
	DIR=''
	CLASS=$1
	DEF="${CLASS^^}_H_"
elif [ $# -eq 2 ]; then
	DIR=$1
	CLASS=$2
	DIRDEF="${DIR^^}"
	DEF="${DIRDEF//\//_}_${CLASS^^}_H_"
else
	echo "Usage: $0 [[directory] Class]"
	exit 1
fi

echo -e "#ifndef $DEF
#define $DEF

class $CLASS {
public:
\t$CLASS();
\t~$CLASS();

private:
\t$CLASS(const $CLASS&);
\t$CLASS& operator=(const $CLASS&);
};

#endif  // $DEF" > $DIR/${CLASS,,}.h


echo -e "#include \"$DIR/${CLASS,,}.h\"

$CLASS::$CLASS()
{
}

$CLASS::~$CLASS()
{
}" > $DIR/${CLASS,,}.cc

git add $DIR/${CLASS,,}.h $DIR/${CLASS,,}.cc

echo "Don't forget to add to the Makefile."
