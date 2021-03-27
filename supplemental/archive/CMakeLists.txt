configure_file(archive.pl			"${ARCHIVE_PROJECT_DIR}/archive.pl" COPYONLY)
configure_file(archive.config.pl.in		"${ARCHIVE_PROJECT_DIR}/archive.config.pl")
configure_file(archive.folders_to_backup.pl.in	"${ARCHIVE_PROJECT_DIR}/archive.folders_to_backup.pl")
configure_file(archive.persistent_folders.pl.in	"${ARCHIVE_PROJECT_DIR}/archive.persistent_folders.pl")

install(FILES
	"${ARCHIVE_PROJECT_DIR}/archive.pl"
	DESTINATION	${ARCHIVE_INSTALL_DIR}
	PERMISSIONS	OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install(FILES
	"${ARCHIVE_PROJECT_DIR}/archive.config.pl"
	"${ARCHIVE_PROJECT_DIR}/archive.folders_to_backup.pl"
	"${ARCHIVE_PROJECT_DIR}/archive.persistent_folders.pl"
	DESTINATION	${ARCHIVE_INSTALL_DIR}
)
