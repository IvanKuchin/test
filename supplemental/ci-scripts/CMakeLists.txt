configure_file(pre.sql.in              "${CI_SCRIPT_PROJECT_DIR}/pre.sql")
configure_file(ci.sql                  "${CI_SCRIPT_PROJECT_DIR}/ci.sql" COPYONLY)
configure_file(mysql_provisioning.in   "${CI_SCRIPT_PROJECT_DIR}/mysql_provisioning")
