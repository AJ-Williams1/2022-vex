#       ports file       | remove comments | del blanks  | no #define          | join lines   | comma-separate  | remove "PORT"    | no trailing , | remove WHEEL
ports="$(cat src/ports.h | sed '/\/\/.*/d' | sed '/^$/d' | awk '{print $2,$3}' | tr '\n' '\\' | sed 's/\\/, /g' | sed 's/_PORT//g' | sed 's/, $//' | sed 's/WHEEL_//g')"

perl -0777 -i -pe "s/.*\"description\".*/      \"description\": \"${ports}\",/" project.pros
