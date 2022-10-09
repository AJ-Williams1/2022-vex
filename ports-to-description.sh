#       ports file       | remove comments | del blanks  | no #define          | join lines   | comma-separate  | remove PORT suffix
ports="$(cat src/ports.h | sed '/\/\/.*/d' | sed '/^$/d' | awk '{print $2,$3}' | tr '\n' '\\' | sed 's/\\/, /g' | sed 's/_PORT//g')"

perl -0777 -i -pe "s/.*\"description\".*/      \"description\": \"${ports}\",/" project.pros
