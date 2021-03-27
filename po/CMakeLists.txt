message(STATUS "Install locales")

configure_file(ru.pot	"${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.pot" COPYONLY)

install(CODE "execute_process(COMMAND msginit --no-translator --locale ru_RU.utf-8 --output ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.po --input ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.pot)")
install(CODE "execute_process(COMMAND msgfmt ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.po -o ${LOCALE_INSTALL_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.mo)")

