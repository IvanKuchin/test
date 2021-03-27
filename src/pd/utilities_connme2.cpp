#include "utilities_connme.h"

static string AdditionalUserData(string userID, CMysql *db, CUser *user)
{
	ostringstream	ost, ostResult;
	auto			affected = 0;
	map<string, string>		skillMap;

				ostResult << "\"school\": [";
				ost.str("");
				ost << "\
	SELECT `users_school`.`id` as 'users_school_id', `school`.`title` as 'school_title', `geo_locality`.`title` as 'school_locality',  `users_school`.`occupation_start` as 'users_school_occupation_start', `users_school`.`occupation_finish` as 'users_school_occupation_finish', \
	`school`.`id` as 'school_id', `school`.`logo_folder` as 'school_logo_folder', `school`.`logo_filename` as 'school_logo_filename' \
	FROM `users_school` \
	RIGHT JOIN `school` ON `users_school`.`school_id`=`school`.`id` \
	RIGHT JOIN `geo_locality` ON `school`.`geo_locality_id`=`geo_locality`.`id` \
	where `users_school`.`user_id`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"schoolID\":\"" << db->Get(i, "users_school_id") << "\","
							<< "\"schoolInternalID\":\"" << db->Get(i, "school_id") << "\","
							<< "\"schoolPhotoFolder\":\"" << db->Get(i, "school_logo_folder") << "\","
							<< "\"schoolPhotoFilename\":\"" << db->Get(i, "school_logo_filename") << "\","
							<< "\"schoolTitle\":\"" << db->Get(i, "school_title") << "\","
							<< "\"schoolLocality\":\"" << db->Get(i, "school_locality") << "\","
							<< "\"schoolOccupationStart\":\"" << db->Get(i, "users_school_occupation_start") << "\","
							<< "\"schoolOccupationFinish\":\"" << db->Get(i, "users_school_occupation_finish") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "school path is empty");
				}
				ostResult << "],";

/*
				ostResult << "\"language\": [";
				ost.str("");
				ost << "SELECT `users_language`.`id` as 'users_language_id', `users_language`.`level` as 'language_level', ";
				ost << "`language`.`id` as 'language_id', `language`.`title` as 'language_title', ";
				ost << "`language`.`logo_folder` as 'language_logo_folder', `language`.`logo_filename` as 'language_logo_filename' ";
				ost << "FROM `users_language` ";
				ost << "RIGHT JOIN `language` ON `users_language`.`language_id`=`language`.`id` ";
				ost << "WHERE `users_language`.`user_id`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"languageID\":\"" << db->Get(i, "users_language_id") << "\","
							<< "\"languageInternalID\":\"" << db->Get(i, "language_id") << "\","
							<< "\"languagePhotoFolder\":\"" << db->Get(i, "language_logo_folder") << "\","
							<< "\"languagePhotoFilename\":\"" << db->Get(i, "language_logo_filename") << "\","
							<< "\"languageTitle\":\"" << db->Get(i, "language_title") << "\","
							<< "\"languageLevel\":\"" << db->Get(i, "language_level") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "language path is empty");
				}
				ostResult << "],";

				skillMap.clear();
				ostResult << "\"skill\": [";
				ost.str("");
				ost << "\
	SELECT `users_skill`.`id` as 'users_skill_id', `skill`.`title` as 'skill_title' \
	FROM `users_skill` \
	RIGHT JOIN `skill` ON `users_skill`.`skill_id`=`skill`.`id` \
	where `users_skill`.`user_id`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							skillMap[db->Get(i, "users_skill_id")] = db->Get(i, "skill_title");
						}
						for(auto it = skillMap.begin(); it != skillMap.end(); ++it) 
						{
							ostResult << ((it != skillMap.begin()) ? "," : "");
							ostResult << "{";
							ostResult << "\"skillID\":\"" << it->first << "\",";
							ostResult << "\"skillTitle\":\"" << it->second << "\",";

							ostResult << "\"skillConfirmed\":[";
							ost.str("");
							ost << "SELECT * FROM `skill_confirmed` WHERE `users_skill_id`=\"" << it->first << "\";";
							if(int affected1 = db->Query(ost.str()))
							{
								for(auto i = 0; i < affected1; ++i)
								{
									ostResult << (i ? "," : "");
									ostResult << db->Get(i, "approver_userID");
								}
							}
							ostResult << "]";

							ostResult << "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "language path is empty");
				}
				ostResult << "],";
*/
				ostResult << "\"university\": [";
				ost.str("");
				ost << "\
	SELECT `users_university`.`id` as 'users_university_id', `university`.`title` as 'university_title', `users_university`.`degree` as 'university_degree', `geo_region`.`title` as 'university_region', `users_university`.`occupation_start` as 'users_university_occupation_start', `users_university`.`occupation_finish` as 'users_university_occupation_finish', \
	`university`.`id` as 'university_id', `university`.`logo_folder` as 'university_logo_folder', `university`.`logo_filename` as 'university_logo_filename' \
	FROM `users_university` \
	RIGHT JOIN `university` ON `users_university`.`university_id`=`university`.`id` \
	RIGHT JOIN `geo_region` ON `university`.`geo_region_id`=`geo_region`.`id` \
	where `users_university`.`user_id`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"universityID\":\"" << db->Get(i, "users_university_id") << "\","
							<< "\"universityInternalID\":\"" << db->Get(i, "university_id") << "\","
							<< "\"universityPhotoFolder\":\"" << db->Get(i, "university_logo_folder") << "\","
							<< "\"universityPhotoFilename\":\"" << db->Get(i, "university_logo_filename") << "\","
							<< "\"universityTitle\":\"" << db->Get(i, "university_title") << "\","
							<< "\"universityRegion\":\"" << db->Get(i, "university_region") << "\","
							<< "\"universityDegree\":\"" << db->Get(i, "university_degree") << "\","
							<< "\"universityOccupationStart\":\"" << db->Get(i, "users_university_occupation_start") << "\","
							<< "\"universityOccupationFinish\":\"" << db->Get(i, "users_university_occupation_finish") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "university path is empty");
				}
				ostResult << "],";

				ostResult << "\"certifications\": [";
				ost.str("");
				ost << "SELECT `users_certifications`.`id` as 'users_certifications_id', `company`.`name` as 'certification_vendors_title', `certification_tracks`.`title` as 'certification_tracks_title', `users_certifications`.`certification_number` as 'users_certifications_certification_number', ";
				ost << "`certification_tracks`.`id` as 'certification_id', `certification_tracks`.`logo_folder` as 'certification_logo_folder', `certification_tracks`.`logo_filename` as 'certification_logo_filename' ";
				ost << "FROM `users_certifications` ";
				ost << "RIGHT JOIN `certification_tracks` on `users_certifications`.`track_id`=`certification_tracks`.`id` ";
				ost << "RIGHT JOIN `company` ON `certification_tracks`.`vendor_id`=`company`.`id` ";
				ost << "WHERE `users_certifications`.`user_id`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"certificationID\":\"" << db->Get(i, "users_certifications_id") << "\","
							<< "\"certificationInternalID\":\"" << db->Get(i, "certification_id") << "\","
							<< "\"certificationPhotoFolder\":\"" << db->Get(i, "certification_logo_folder") << "\","
							<< "\"certificationPhotoFilename\":\"" << db->Get(i, "certification_logo_filename") << "\","
							<< "\"certificationVendor\":\"" << db->Get(i, "certification_vendors_title") << "\","
							<< "\"certificationTrack\":\"" << db->Get(i, "certification_tracks_title") << "\","
							<< "\"certificationNumber\":\"" << db->Get(i, "users_certifications_certification_number") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "certification path is empty");
				}
				ostResult << "],";

				ostResult << "\"courses\": [";
				ost.str("");
				ost << "SELECT `users_courses`.`id` as 'users_courses_id', `company`.`name` as 'course_vendors_title', `certification_tracks`.`title` as 'course_tracks_title', `users_courses`.`rating` as 'users_courses_rating',  `users_courses`.`eventTimestamp` as 'users_courses_eventtimestamp', ";
				ost << "`certification_tracks`.`id` as 'course_id', `certification_tracks`.`logo_folder` as 'course_logo_folder', `certification_tracks`.`logo_filename` as 'course_logo_filename' ";
				ost << "FROM `users_courses` ";
				ost << "RIGHT JOIN `certification_tracks` on `users_courses`.`track_id`=`certification_tracks`.`id` ";
				ost << "RIGHT JOIN `company` ON `certification_tracks`.`vendor_id`=`company`.`id` ";
				ost << "WHERE `users_courses`.`user_id`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"courseID\":\"" << db->Get(i, "users_courses_id") << "\","
							<< "\"courseInternalID\":\"" << db->Get(i, "course_id") << "\","
							<< "\"coursePhotoFolder\":\"" << db->Get(i, "course_logo_folder") << "\","
							<< "\"coursePhotoFilename\":\"" << db->Get(i, "course_logo_filename") << "\","
							<< "\"courseVendor\":\"" << db->Get(i, "course_vendors_title") << "\","
							<< "\"courseTrack\":\"" << db->Get(i, "course_tracks_title") << "\","
							<< "\"courseRating\":\"" << db->Get(i, "users_courses_rating") << "\","
							<< "\"eventTimestamp\":\"" << db->Get(i, "users_courses_eventtimestamp") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "course path is empty");
				}
				ostResult << "],";

				ostResult << "\"books\": [";
				ost.str("");
				ost << "SELECT `users_books`.`id` as 'users_books_id', `book`.`id` as 'book_id', `book`.`title` as 'book_title', `book`.`isbn10` as 'book_isbn10', `book`.`isbn13` as 'book_isbn13', `book`.`coverPhotoFolder` as 'book_coverPhotoFolder', `book`.`coverPhotoFilename` as 'book_coverPhotoFilename',  "
						"`book_author`.`name` as `book_author_name`, `users_books`.`rating` as `users_books_rating` , `users_books`.`bookReadTimestamp` as `users_books_bookReadTimestamp` "
						"FROM `users_books` "
						"RIGHT JOIN `book`		ON `users_books`.`bookID`=`book`.`id` "
						"RIGHT JOIN `book_author` ON `book`.`authorID`=`book_author`.`id` "
						"where `users_books`.`userID`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"id\":\"" << db->Get(i, "users_books_id") << "\","
							<< "\"bookID\":\"" << db->Get(i, "book_id") << "\","
							<< "\"bookTitle\":\"" << db->Get(i, "book_title") << "\","
							<< "\"bookAuthorName\":\"" << db->Get(i, "book_author_name") << "\","
							<< "\"bookRating\":\"" << db->Get(i, "users_books_rating") << "\","
							<< "\"bookReadTimestamp\":\"" << db->Get(i, "users_books_bookReadTimestamp") << "\","
							<< "\"bookISBN10\":\"" << db->Get(i, "book_isbn10") << "\","
							<< "\"bookISBN13\":\"" << db->Get(i, "book_isbn13") << "\","
							<< "\"bookCoverPhotoFolder\":\"" << db->Get(i, "book_coverPhotoFolder") << "\","
							<< "\"bookCoverPhotoFilename\":\"" << db->Get(i, "book_coverPhotoFilename") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "book path is empty");
				}
				ostResult << "],";

				ostResult << "\"vacancyApplied\": [";
				affected = db->Query("SELECT * FROM `company_candidates` WHERE `user_id`=\"" + userID + "\";");
				if(affected)
				{
						struct ItemClass
						{
							string		id;
							string		vacancy_id;
							string		user_id;
							string		answer1;
							string		answer2;
							string		answer3;
							string		language1;
							string		language2;
							string		language3;
							string		skill1;
							string		skill2;
							string		skill3;
							string		description;
							string		status;
							string		eventTimestamp;
						};

						vector<ItemClass>		itemsList;
						int						itemsCount = affected;
						string					tempBuffer = "";

						for(auto i = 0; i < itemsCount; ++i)
						{
							ItemClass   item;

							item.id = db->Get(i, "id");
							item.vacancy_id = db->Get(i, "vacancy_id");
							item.user_id = db->Get(i, "user_id");
							item.answer1 = db->Get(i, "answer1");
							item.answer2 = db->Get(i, "answer2");
							item.answer3 = db->Get(i, "answer3");
							item.language1 = db->Get(i, "language1");
							item.language2 = db->Get(i, "language2");
							item.language3 = db->Get(i, "language3");
							item.skill1 = db->Get(i, "skill1");
							item.skill2 = db->Get(i, "skill2");
							item.skill3 = db->Get(i, "skill3");
							item.description = db->Get(i, "description");
							item.status = db->Get(i, "status");
							item.eventTimestamp = db->Get(i, "eventTimestamp");

							itemsList.push_back(item);
						}

						for(auto i = 0; i < affected; i++) 
						{

							if(db->Query("SELECT * FROM `company_vacancy` WHERE `id`=\"" + itemsList[i].vacancy_id + "\";"))
							{
								string 		company_id = db->Get(0, "company_id");

								if(tempBuffer.length()) tempBuffer += ",";

								tempBuffer += "{"
											"\"id\":\"" + itemsList[i].id + "\","
											"\"vacancy\":[" + GetOpenVacanciesInJSONFormat("SELECT * FROM `company_vacancy` WHERE `id`=\"" + itemsList[i].vacancy_id + "\";", db) + "],"
											"\"company\":[" + GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id`=\"" + company_id + "\";", db, NULL) + "],"
											"\"user_id\":\"" + itemsList[i].user_id + "\","
											"\"answer1\":\"" + itemsList[i].answer1 + "\","
											"\"answer2\":\"" + itemsList[i].answer2 + "\","
											"\"answer3\":\"" + itemsList[i].answer3 + "\","
											"\"language1\":\"" + itemsList[i].language1 + "\","
											"\"language2\":\"" + itemsList[i].language2 + "\","
											"\"language3\":\"" + itemsList[i].language3 + "\","
											"\"skill1\":\"" + itemsList[i].skill1 + "\","
											"\"skill2\":\"" + itemsList[i].skill2 + "\","
											"\"skill3\":\"" + itemsList[i].skill3 + "\","
											"\"description\":\"" + itemsList[i].description + "\","
											"\"status\":\"" + itemsList[i].status + "\","
											"\"eventTimestamp\":\"" + itemsList[i].eventTimestamp + "\""
											"}";
							}
							else
							{
								MESSAGE_ERROR("", "", "can't find vacancy.id[" + itemsList[i].vacancy_id + "]");
							}

						}
						ostResult << tempBuffer;
				}
				else 
				{
					MESSAGE_DEBUG("", "", "recommendation path is empty");
				}
				ostResult << "],";

				ostResult << "\"recommendation\": [";
				ost.str("");
				ost << "SELECT * FROM `users_recommendation` WHERE `recommended_userID`=\"" << userID << "\";";
				affected = db->Query(ost.str());
				if(affected)
				{
						for(auto i = 0; i < affected; i++) 
						{
							ostResult << (i ? "," : "")
							<< "{"
							<< "\"recommendationID\":\"" << db->Get(i, "id") << "\","
							<< "\"recommendationTitle\":\"" << db->Get(i, "title") << "\","
							<< "\"recommendationRecommendedUserID\":\"" << db->Get(i, "recommended_userID") << "\","
							<< "\"recommendationRecommendingUserID\":\"" << db->Get(i, "recommending_userID") << "\","
							<< "\"recommendationTimestamp\":\"" << db->Get(i, "eventTimestamp") << "\","
							<< "\"recommendationState\":\"" << db->Get(i, "state") << "\""
							<< "}";
						}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "recommendation path is empty");
				}
				ostResult << "],";

				ostResult << "\"companies\": [";
				ost.str("");
				ost << "SELECT `users_company`.`id` as `users_company_id`, `company`.`id` as `company_id`, `company`.`name` as `company_name`,`company`.`type` as `company_type`, `occupation_start`, `occupation_finish`, `current_company`, `responsibilities`, `company_position`.`title` as `title`, \
		`company`.`logo_folder` as `company_logo_folder`,`company`.`logo_filename` as `company_logo_filename` \
		FROM  `company` ,  `users_company` ,  `company_position` \
		WHERE  `user_id` =  '" << userID << "' \
		AND  `company`.`id` =  `users_company`.`company_id`  \
		AND  `company_position`.`id` =  `users_company`.`position_title_id`  \
		ORDER BY  `users_company`.`occupation_start` DESC ";
				affected = db->Query(ost.str());
				if(affected > 0) 
				{
					for(auto i = 0; i < affected; i++) 
					{
						ostResult << (i ? "," : "")
						<< "{"
						<< "\"companyID\":\"" << db->Get(i, "users_company_id") << "\","
						<< "\"companyInternalID\":\"" << db->Get(i, "company_id") << "\","
						<< "\"companyType\":\"" << db->Get(i, "company_type") << "\","
						<< "\"companyName\":\"" << db->Get(i, "company_name") << "\","
						<< "\"companyLogoFolder\":\"" << db->Get(i, "company_logo_folder") << "\","
						<< "\"companyLogoFilename\":\"" << db->Get(i, "company_logo_filename") << "\","
						<< "\"occupationStart\":\"" << db->Get(i, "occupation_start") << "\","
						<< "\"occupationFinish\":\"" << db->Get(i, "occupation_finish") << "\","
						<< "\"currentCompany\":\"" << db->Get(i, "current_company") << "\","
						<< "\"title\":\"" << db->Get(i, "title") << "\","
						<< "\"responsibilities\":\"" << db->Get(i, "responsibilities") << "\""
						<< "}";
					}
				}
				else 
				{
					MESSAGE_DEBUG("", "", "carrier path is empty");
				}
				ostResult << "]";

	return ostResult.str();
}

auto GetUsersID_BySearchString(const string &lookForKey, bool include_myself, CMysql *db, CUser *user) -> vector<string>
{
	MESSAGE_DEBUG("", "", "start(" + lookForKey + ")");

	vector<string>	result, searchWords;

	if(qw(lookForKey, searchWords))
	{
		// --- single search word can be name, surname, company name 
		if(searchWords.size() == 1)		
		{
			MESSAGE_DEBUG("", "", "single word search");

			// --- Looking through name, last name
			auto query = 
					"SELECT `users`.`id` FROM `users`"
					"LEFT JOIN `users_company` on `users_company`.`user_id` = `users`.`id` "
					"LEFT JOIN `company` on `company`.`id`=`users_company`.`company_id` "
					"WHERE "
					"("
						" `users`.`isActivated`='Y' AND `users`.`isblocked`='N' "
						+ (include_myself ? "" : " AND `users`.`id`!=\"" + user->GetID() + "\" ") +
					") "
					" AND "
					"("
						"("
							"`users`.`name` LIKE \"%" 	+ lookForKey + "%\" or `users`.`nameLast` LIKE \"%" 	+ lookForKey + "%\""
						")"
						" OR "
						"("
							"`users_company`.`current_company`='1' AND `company`.`name` LIKE \"%" + lookForKey + "%\""
						")"
					")";

			result = GetValuesFromDB(query, db);
		}
		if(searchWords.size() == 2)
		{
			MESSAGE_DEBUG("", "", "two words search");

			auto query = 
					"SELECT `users`.`id` FROM `users` "
					" LEFT JOIN `users_company` on `users_company`.`user_id` = `users`.`id` "
					" LEFT JOIN `company` on `company`.`id`=`users_company`.`company_id` "
					" WHERE "
					"("
						" `users`.`isActivated`='Y' AND `users`.`isblocked`='N' "
						+ (include_myself ? "" : " AND `users`.`id`!=\"" + user->GetID() + "\" ") +
					") "
					" AND "
					"("
						"("
							" `users_company`.`current_company`='1' "
							" AND "
							"( "
							" 	`company`.`name` like \"%" 	+ searchWords[0] + "%\" or "
							"	`company`.`name` like \"%" 	+ searchWords[1] + "%\" "
							")"
							" AND "
							"( "
							" 	`users`.`name` like \"%" 		+ searchWords[0] + "%\" or "
							" 	`users`.`name` like \"%" 		+ searchWords[1] + "%\" or "
							" 	`users`.`nameLast` like \"%" 	+ searchWords[0] + "%\" or "
							" 	`users`.`nameLast` like \"%" 	+ searchWords[1] + "%\" "
							" )"
						")"
						" OR "
						"("
							" ( "
							" 	`users`.`name` like \"%" 		+ searchWords[1] + "%\" AND "
							" 	`users`.`nameLast` like \"%" 	+ searchWords[0] + "%\" "
							" ) "
							" or "
							" ( "
							" 	`users`.`name` like \"%" 		+ searchWords[0] + "%\" AND "
							" 	`users`.`nameLast` like \"%" 	+ searchWords[1] + "%\" "
							" ) "
						")"
					")";

			result = GetValuesFromDB(query, db);
		}
		if(searchWords.size() == 3)
		{
			MESSAGE_DEBUG("", "", "three words search");

			auto	query = 
					"SELECT `users`.`id` FROM `users` "
					" LEFT JOIN `users_company` on `users_company`.`user_id` = `users`.`id` "
					" LEFT JOIN `company` on `company`.`id`=`users_company`.`company_id` "
					" WHERE "
					"("
						" `users`.`isActivated`='Y' AND `users`.`isblocked`='N' "
						+ (include_myself ? "" : " AND `users`.`id`!=\"" + user->GetID() + "\" ") +
					") "
					" AND "
					"("
						"("
							"`users_company`.`current_company`='1' and "
							"( "
							"	`company`.`name` like \"%" 		+ searchWords[0] + "%\" or "
							"	`company`.`name` like \"%" 		+ searchWords[1] + "%\" or "
							"	`company`.`name` like \"%" 		+ searchWords[2] + "%\" "
							")"
							" AND "
							"( "
							"	`users`.`name` like \"%" 		+ searchWords[0] + "%\" or "
							"	`users`.`name` like \"%" 		+ searchWords[1] + "%\" or "
							"	`users`.`name` like \"%" 		+ searchWords[2] + "%\" or "
							"	`users`.`nameLast` like \"%" 	+ searchWords[0] + "%\" or "
							"	`users`.`nameLast` like \"%" 	+ searchWords[1] + "%\" or "
							"	`users`.`nameLast` like \"%" 	+ searchWords[2] + "%\""
							")"
						")"
						" OR "
						"("
							"("
							"	`users`.`name` like \"%" 		+ searchWords[1] + "%\" and "
							"	`users`.`nameLast` like \"%" 	+ searchWords[0] + "%\" "
							")"
							" OR "
							"("
							"	`users`.`name` like \"%" 		+ searchWords[0] + "%\" and "
							"	`users`.`nameLast` like \"%" 	+ searchWords[1] + "%\" "
							")"
							" OR "
							"("
							"	`users`.`name` like \"%" 		+ searchWords[2] + "%\" and "
							"	`users`.`nameLast` like \"%" 	+ searchWords[0] + "%\" "
							")"
							" OR "
							"("
							"	`users`.`name` like \"%" 		+ searchWords[0] + "%\" and "
							"	`users`.`nameLast` like \"%" 	+ searchWords[2] + "%\" "
							")"
							" OR "
							"("
							"	`users`.`name` like \"%" 		+ searchWords[1] + "%\" and "
							"	`users`.`nameLast` like \"%" 	+ searchWords[2] + "%\" "
							")"
							" OR "
							"("
							"	`users`.`name` like \"%" 		+ searchWords[2] + "%\" and "
							"	`users`.`nameLast` like \"%" 	+ searchWords[1] + "%\" "
							")"
						")"
					")";

			result = GetValuesFromDB(query, db);
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "search request is empty");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto GetUserListInJSONFormat_BySearchString(const string &lookForKey, bool include_myself, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start(" + lookForKey + ")");

	auto			users_id	= GetUsersID_BySearchString(lookForKey, include_myself, db, user);
	auto			result		= (users_id.size() ? GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id` IN (" + join(users_id, ",") + ");", db, user) : "");

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto GetUsersNameAvatarInJSONFormat(string dbQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	unordered_set<unsigned long>	setOfUserID;
	vector<string>					users_vector;

	struct	ItemClass
	{
		string	userID;
		string	userName;
		string	userNameLast;
		string	userSex;
	};
	vector<ItemClass>		itemsList;
	auto					itemsCount = 0;

	if((itemsCount = db->Query(dbQuery)) > 0)
	{
		for(auto i = 0; i < itemsCount; ++i)
		{
			ItemClass	item;
			item.userID								= db->Get(i, "id");
			item.userName							= db->Get(i, "name");
			item.userNameLast						= db->Get(i, "nameLast");
			item.userSex							= db->Get(i, "sex");

			itemsList.push_back(item);
		}


		for(auto i = 0; i < itemsCount; i++) 
		{
			auto					userID		= itemsList[i].userID;
			auto					cache_key	= "SELECT `name`,`nameLast`,`sex` FROM `users` WHERE `id`=" + quoted(userID);
			static c_cache_obj		cache_obj;
			auto					temp_result	= cache_obj.GetFromCache(cache_key);

			if(temp_result.empty())
			{
				if(setOfUserID.find(stol(itemsList[i].userID)) == setOfUserID.end())
				{
					auto		isMe			= user && (userID == user->GetID());
					auto		userName		= itemsList[i].userName;
					auto		userNameLast	= itemsList[i].userNameLast;
					auto		userSex			= itemsList[i].userSex;
					auto		avatarPath		= "empty"s;

					if(db->Query("select * from `users_avatars` where `userid`='" + userID + "' and `isActive`='1';"))
					{
						avatarPath = "/images/avatars/avatars" + db->Get(0, "folder") + "/" + db->Get(0, "filename");
					}

					temp_result = "{"
							  "\"id\": \""							+ userID + "\", "
							  "\"name\": \""						+ userName + "\", "
							  "\"nameLast\": \""					+ userNameLast + "\","
							  "\"sex\": \""							+ userSex + "\","
							  "\"avatar\": \""						+ avatarPath + "\","
							  "\"isMe\": \""						+ (isMe ? "yes" : "no") + "\"" +
							  "}";
				}
				cache_obj.AddToCache(cache_key, temp_result);
			}
			else
			{
				MESSAGE_DEBUG("", "", "user info taken from cache (" + cache_key + ")");
			}
			users_vector.push_back(temp_result);
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "no users found by the query [" + dbQuery + "]");
	}
	
	MESSAGE_DEBUG("", "", "finish");

	return join(users_vector, ",");
}

// --- Returns user list in JSON format grabbed from DB
// --- Input: dbQuery - SQL format returns users
//			db	  - DB connection
//			user	- current user object
string GetUserListInJSONFormat(string dbQuery, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start");

	string							sessid, lookForKey;
	unordered_set<unsigned long>	setOfUserID;
	vector<string>					users_vector;

	struct	ItemClass
	{
		string	userID;
		string	userLogin;
		string	userName;
		string	userNameLast;
		string	userSex;
		string	site_theme_id;
		string	cv;
		string	address;
		string	isBlocked;
		string	phone;
		string	country_code;
		string	email;
		string	email_changeable;
		string	ribbons;
		string	userBirthday;
		string	userBirthdayAccess;
		string	userAppliedVacanciesRender;
		string	userCurrentCity;
		string	userLastOnline;
		string	userLastOnlineSecondSinceY2k;
		string	helpdesk_subscription_S1_sms;
		string	helpdesk_subscription_S2_sms;
		string	helpdesk_subscription_S3_sms;
		string	helpdesk_subscription_S4_sms;
		string	helpdesk_subscription_S1_email;
		string	helpdesk_subscription_S2_email;
		string	helpdesk_subscription_S3_email;
		string	helpdesk_subscription_S4_email;
	};
	vector<ItemClass>		itemsList;
	auto					itemsCount = 0;

	if((itemsCount = db->Query(dbQuery)) > 0)
	{
		for(auto i = 0; i < itemsCount; ++i)
		{
			ItemClass	item;
			item.userID								= db->Get(i, "id");
			item.userLogin							= db->Get(i, "login");
			item.userName							= db->Get(i, "name");
			item.userNameLast						= db->Get(i, "nameLast");
			item.userSex							= db->Get(i, "sex");
			item.cv									= db->Get(i, "cv");
			item.address							= db->Get(i, "address");
			item.isBlocked							= db->Get(i, "isblocked");
			item.email								= db->Get(i, "email");
			item.email_changeable					= db->Get(i, "email_changeable");
			item.phone								= db->Get(i, "phone");
			item.country_code						= db->Get(i, "country_code");
			item.site_theme_id						= db->Get(i, "site_theme_id");
			item.userBirthday						= db->Get(i, "birthday");
			item.userBirthdayAccess					= db->Get(i, "birthdayAccess");
			item.userAppliedVacanciesRender			= db->Get(i, "appliedVacanciesRender");
			item.userCurrentCity					= "не определено";
			item.userLastOnline						= db->Get(i, "last_online");
			item.userLastOnlineSecondSinceY2k		= db->Get(i, "last_onlineSecondsSinceY2k");
			item.helpdesk_subscription_S1_email		= db->Get(i, "helpdesk_subscription_S1_email");
			item.helpdesk_subscription_S2_email		= db->Get(i, "helpdesk_subscription_S2_email");
			item.helpdesk_subscription_S3_email		= db->Get(i, "helpdesk_subscription_S3_email");
			item.helpdesk_subscription_S4_email		= db->Get(i, "helpdesk_subscription_S4_email");
			item.helpdesk_subscription_S1_sms		= db->Get(i, "helpdesk_subscription_S1_sms");
			item.helpdesk_subscription_S2_sms		= db->Get(i, "helpdesk_subscription_S2_sms");
			item.helpdesk_subscription_S3_sms		= db->Get(i, "helpdesk_subscription_S3_sms");
			item.helpdesk_subscription_S4_sms		= db->Get(i, "helpdesk_subscription_S4_sms");

			itemsList.push_back(item);
		}


		for(auto i = 0; i < itemsCount; i++) 
		{
			auto					userID		= itemsList[i].userID;
			auto					cache_key	= "SELECT * FROM `users` WHERE `id`=" + quoted(userID);
			static c_cache_obj		cache_obj;
			auto					temp_result	= cache_obj.GetFromCache(cache_key);

			if(temp_result.empty())
			{
				if(setOfUserID.find(stol(itemsList[i].userID)) == setOfUserID.end())
				{
					string				userLogin, userName, userNameLast, userSex, userBirthday, userBirthdayAccess, userCurrentEmployment, userCurrentCity, avatarPath;
					string				userAppliedVacanciesRender;
					string				userLastOnline, numberUreadMessages, userLastOnlineSecondSinceY2k;
					string				userFriendship;
					ostringstream		ost1;
					int					affected1;
					auto				isMe					= user && (userID == user->GetID());

					userLogin = itemsList[i].userLogin;
					userName = itemsList[i].userName;
					userNameLast = itemsList[i].userNameLast;
					userSex = itemsList[i].userSex;
					userBirthday = itemsList[i].userBirthday;
					userBirthdayAccess = itemsList[i].userBirthdayAccess;
					userAppliedVacanciesRender = itemsList[i].userAppliedVacanciesRender;
					userCurrentCity = itemsList[i].userCurrentCity;
					userLastOnline = itemsList[i].userLastOnline;
					userLastOnlineSecondSinceY2k = itemsList[i].userLastOnlineSecondSinceY2k;

					setOfUserID.insert(atol(userID.c_str()));

					// --- Defining title and company of user
					ost1.str("");
					ost1 << "SELECT `company_position`.`title` as `users_company_position_title`,  \
							`company`.`name` as `company_name`, `company`.`id` as `company_id`  \
							FROM `users_company` \
							LEFT JOIN  `company_position` ON `company_position`.`id`=`users_company`.`position_title_id` \
							LEFT JOIN  `company` 				ON `company`.`id`=`users_company`.`company_id` \
							WHERE `users_company`.`user_id`=\"" << userID << "\" and `users_company`.`current_company`='1' \
							ORDER BY  `users_company`.`occupation_start` DESC ";

					affected1 = db->Query(ost1.str());
					ost1.str("");
					ost1 << "[";
					if(affected1 > 0)
					{
						for(int j = 0; j < affected1; j++)
						{
							ost1 << "{"
									"\"companyID\": \""	<< db->Get(j, "company_id")						<< "\","
									"\"company\": \""	<< db->Get(j, "company_name")					<< "\","
									"\"title\": \""		<< db->Get(j, "users_company_position_title")	<< "\""
									"}";
							if(j < (affected1 - 1)) ost1 << ", ";
						}
					}
					ost1 << "]";
					userCurrentEmployment = ost1.str(); 

					{
						CLog	log;

						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: done with building employment list ", userCurrentEmployment);
					}

					// --- Get user avatars
					avatarPath = "empty";
					if(db->Query("select * from `users_avatars` where `userid`='" + userID + "' and `isActive`='1';"))
						avatarPath = "/images/avatars/avatars" + db->Get(0, "folder") + "/" + db->Get(0, "filename");

					// --- Get friendship status
					userFriendship = "empty";
					if(user && db->Query("select * from `users_friends` where `userid`='" + user->GetID() + "' and `friendID`='" + userID + "';"))
					{
						userFriendship = db->Get(0, "state");
					}

					// --- Get presence status for chat purposes
					ost1.str("");
					ost1 << "select COUNT(*) as `number_unread_messages` from `chat_messages` where `fromType`='fromUser' and `fromID`='" << userID << "' and (`messageStatus`='unread' or `messageStatus`='sent' or `messageStatus`='delivered');";
					if(db->Query(ost1.str()))
					{
						numberUreadMessages = db->Get(0, "number_unread_messages");
					}

					if(userBirthdayAccess == "private") userBirthday = "";

					temp_result = "{"
							  "\"id\": \""							+ userID + "\", "
							  "\"userID\": \""						+ userID + "\", "
							  "\"name\": \""						+ userName + "\", "
							  "\"nameLast\": \""					+ userNameLast + "\","
							  "\"cv\": \""							+ itemsList[i].cv + "\", "
							  "\"address\": \""						+ itemsList[i].address + "\", "
							  "\"userSex\": \""						+ userSex + "\","
							  "\"sex\": \""							+ userSex + "\","
							  "\"birthday\": \""					+ userBirthday + "\","
							  "\"birthdayAccess\": \""				+ userBirthdayAccess + "\","
							  "\"appliedVacanciesRender\": \""		+ userAppliedVacanciesRender + "\","
							  "\"last_online\": \""					+ userLastOnline + "\","
							  "\"last_online_diff\": \""			+ to_string(GetTimeDifferenceFromNow(userLastOnline)) + "\","
							  "\"last_onlineSecondsSinceY2k\": \""  + userLastOnlineSecondSinceY2k + "\","
							  "\"userFriendship\": \""				+ userFriendship + "\","
							  "\"avatar\": \""						+ avatarPath + "\","
							  "\"ribbons\": ["						+ GetUserRibbons_InJSONFormat("SELECT * FROM `users_ribbons` WHERE `id` IN (" + Get_UserRibbonsIDByUserID_sqlquery(userID) + ");", db) + "],"
							  "\"currentEmployment\": "				+ userCurrentEmployment + ", "
							  "\"currentCity\": \""					+ userCurrentCity + "\", "
							  "\"numberUnreadMessages\": \""		+ numberUreadMessages + "\", "
							  // "\"languages\": ["		 			+ GetLanguageListInJSONFormat("SELECT * FROM `language` WHERE `id` in (SELECT `language_id` FROM `users_language` WHERE `user_id`=\"" + userID + "\");", db) + "], "
							  "\"languages\": ["		 			+ GetUserLanguageListInJSONFormat(userID, db) + "], "
							  "\"skills\": ["		 				+ GetSkillListInJSONFormat(userID, db) + "], "
							  "\"site_theme_id\": \""				+ itemsList[i].site_theme_id + "\","
							  "\"themes\": ["						+ GetSiteThemesInJSONFormat("SELECT * FROM `site_themes`", db, user) + "],"
							  "\"country_code\": \""				+ (isMe ? itemsList[i].country_code : "") + "\","
							  "\"phone\": \""						+ (isMe ? itemsList[i].phone: "") + "\","
							  "\"email\": \""						+ (isMe ? itemsList[i].email : "") + "\","
							  "\"email_changeable\": \""			+ (isMe ? itemsList[i].email_changeable : "") + "\","
							  "\"helpdesk_subscriptions_sms\": ["	+ (isMe ? quoted(itemsList[i].helpdesk_subscription_S1_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S2_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S3_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S4_sms)  : "") + "],"
							  "\"helpdesk_subscriptions_email\": ["	+ (isMe ? quoted(itemsList[i].helpdesk_subscription_S1_email) + "," + quoted(itemsList[i].helpdesk_subscription_S2_email) + "," + quoted(itemsList[i].helpdesk_subscription_S3_email) + "," + quoted(itemsList[i].helpdesk_subscription_S4_email)  : "") + "],"
							  "\"subscriptions\":[" 				+ GetUserSubscriptionsInJSONFormat("SELECT * FROM `users_subscriptions` WHERE `user_id`=\"" + userID + "\";", db) + "],"
							  "\"groups\":[" 						+ GetGroupListInJSONFormat("SELECT * FROM `groups` WHERE `id` IN (" + Get_Groups_UserSubscribedTo_sqlquery(userID) + ");", db, user) + "],"
							  "\"subscribed_companies\":[" 			+ GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id` IN (" + Get_Companies_UserSubscribedTo_sqlquery(userID) + ");", db, user) + "],"
							  "\"isBlocked\": \""					+ itemsList[i].isBlocked + "\","
							  "\"isMe\": \""						+ (isMe ? "yes" : "no") + "\"," +
							  AdditionalUserData(userID, db, user) +
							  "}";
				} // --- if user is not dupicated
				cache_obj.AddToCache(cache_key, temp_result);
			}
			else
			{
				MESSAGE_DEBUG("", "", "user info taken from cache (" + cache_key + ")");
			}
			users_vector.push_back(temp_result);
		} // --- for loop through user list
	}
	else
	{
		MESSAGE_DEBUG("", "", "no users found by the query [" + dbQuery + "]");
	}
	
	MESSAGE_DEBUG("", "", "finish");

	return join(users_vector, ",");
}


auto GetGroupListInJSONFormat(string dbQuery, CMysql *db, CUser *user) -> string
{
	struct ItemClass 
	{
		string	id;
		string	link;
		string	title;
		string	description;
		string	logo_folder;
		string	logo_filename;
		string	owner_id;
		string	isBlocked;
		string	eventTimestampCreation;
		string	eventTimestampLastPost;
	};

	ostringstream			ost, ostFinal;
	string					sessid, lookForKey;
	int						affected;
	vector<ItemClass>		groupsList;

	MESSAGE_DEBUG("", "", "start");

	ostFinal.str("");

	if((affected = db->Query(dbQuery)) > 0)
	{
		int						groupCounter = affected;

		groupsList.reserve(groupCounter);  // --- reserving allows avoid moving vector in memory
											// --- to fit vector into continuous memory piece

		for(auto i = 0; i < affected; i++)
		{
			ItemClass	group;

			group.id = db->Get(i, "id");
			group.link = db->Get(i, "link");
			group.title = db->Get(i, "title");
			group.description = db->Get(i, "description");
			group.logo_folder = db->Get(i, "logo_folder");
			group.logo_filename = db->Get(i, "logo_filename");
			group.owner_id = db->Get(i, "owner_id");
			group.isBlocked = db->Get(i, "isBlocked");
			group.eventTimestampCreation = db->Get(i, "eventTimestampCreation");
			group.eventTimestampLastPost = db->Get(i, "eventTimestampLastPost");

			groupsList.push_back(group);
		}

		for(auto i = 0; i < groupCounter; i++)
		{
				auto		numberOfMembers				= GetValueFromDB("SELECT COUNT(*) FROM `users` WHERE `id` IN (" + Get_UserIDByGroupID_sqlquery(groupsList[i].id) + ");", db);

				if(ostFinal.str().length()) ostFinal << ", ";

				ostFinal << "{";
				ostFinal << "\"id\": \""				  	<< groupsList[i].id << "\",";
				ostFinal << "\"link\": \""					<< groupsList[i].link << "\",";
				ostFinal << "\"title\": \""					<< groupsList[i].title << "\",";
				ostFinal << "\"description\": \""			<< groupsList[i].description << "\",";
				ostFinal << "\"logo_folder\": \""			<< groupsList[i].logo_folder << "\",";
				ostFinal << "\"logo_filename\": \""			<< groupsList[i].logo_filename << "\",";
				ostFinal << "\"isMine\": \""				<< (user ? groupsList[i].owner_id == user->GetID() : false) << "\",";

				ostFinal << "\"numberOfMembers\": \""		<< numberOfMembers << "\",";
				ostFinal << "\"subscribers\": ["			<< GetUsersNameAvatarInJSONFormat("SELECT * FROM `users` WHERE `id` IN (" + Get_UserIDByGroupID_sqlquery(groupsList[i].id) + ") ORDER BY RAND() LIMIT 20;", db, user) << "],";

				ostFinal << "\"isBlocked\": \""				<< groupsList[i].isBlocked << "\",";
				ostFinal << "\"eventTimestampCreation\": \""<< groupsList[i].eventTimestampCreation << "\",";
				ostFinal << "\"eventTimestampLastPost\": \""<< groupsList[i].eventTimestampLastPost << "\"";
				ostFinal << "}";
		} // --- for loop through group list
	} // --- if sql-query on group selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no groups returned by request [" + dbQuery + "]");
	}

	MESSAGE_DEBUG("", "", "end (result length = " + to_string(ostFinal.str().length()) + ")");

	return ostFinal.str();
}

string GetUnreadChatMessagesInJSONFormat(CUser *user, CMysql *db)
{
	ostringstream	result, ost;
	int				affected;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	result.str("");

	affected = db->Query("select * from `chat_messages` where `toID`='" + user->GetID() + "' and (`messageStatus`='unread' or `messageStatus`='delivered' or `messageStatus`='sent');");
	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			result << (i ? "," : "") << "{"
				"\"id\": \""				<< db->Get(i, "id") << "\", "
				"\"message\": \"" 			<< ReplaceDoubleQuoteToQuote(db->Get(i, "message")) << "\", "
				"\"fromType\": \"" 			<< db->Get(i, "fromType") << "\","
				"\"fromID\": \""			<< db->Get(i, "fromID") << "\","
				"\"toType\": \""			<< db->Get(i, "toType") << "\","
				"\"toID\": \""	 			<< db->Get(i, "toID") << "\","
				"\"messageType\": \""		<< db->Get(i, "messageType") << "\","
				"\"messageStatus\": \""		<< db->Get(i, "messageStatus") << "\","
				"\"eventTimestamp\": \""	<< db->Get(i, "eventTimestamp") << "\""
			"}";
		}
	}
	
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end");
	}

	return	result.str();
}

string GetCompanyPositionIDByTitle(string positionTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			positionID = "";

	MESSAGE_DEBUG("", "", "start");

	if(positionTitle.length())
	{
		if(db->Query("SELECT `id` FROM `company_position` WHERE `title`=\"" + positionTitle + "\";"))
		{
			positionID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "company position not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `company_position` SET `area`=\"\", `title`=\"" + positionTitle + "\";");
			if(tmp)
				positionID = to_string(tmp);
			else
			{
				MESSAGE_ERROR("", "", "fail to insert to company_position table");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "positionTitle is empty");
	}

	result = positionID;

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

string GetLanguageIDByTitle(string languageTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			languageID = "0";

	MESSAGE_DEBUG("", "", "start");

	if(languageTitle.length())
	{
		if(db->Query("SELECT `id` FROM `language` WHERE `title`=\"" + languageTitle + "\";"))
		{
			languageID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "languageTitle [" + languageTitle + "] not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `language` SET `title`=\"" + languageTitle + "\";");
			if(tmp)
				languageID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into language");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "languageTitle is empty");
	}

	result = languageID;

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");


	return result;
}

string GetSkillIDByTitle(string skillTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			languageID = "0";

	MESSAGE_DEBUG("", "", "start");

	if(skillTitle.length())
	{
		if(db->Query("SELECT `id` FROM `skill` WHERE `title`=\"" + skillTitle + "\";"))
		{
			languageID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "skillTitle [" + skillTitle + "] not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `skill` SET `title`=\"" + skillTitle + "\";");
			if(tmp)
				languageID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into skill");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "skillTitle is empty");
	}

	result = languageID;

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

// --- Function returns array of book rating
// --- input: bookID, db
// --- output: book rating array
string GetBookRatingList(string bookID, CMysql *db)
{
	int			 affected;
	string		  result = "";

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("select * from `users_books` where `bookID`=\"" + bookID + "\";");
	if(affected > 0)
	{
		for(auto i = 0; i < affected; ++i)
		{
			if(i) result += ",";
			result += db->Get(i, "rating");
		}
	}

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

// --- Function returns array of course rating
// --- input: courseID, db
// --- output: course rating array
string GetCourseRatingList(string courseID, CMysql *db)
{
	int				affected;
	string			result = "";

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("select * from `users_courses` where `track_id`=\"" + courseID + "\";");
	if(affected > 0)
	{
		for(auto i = 0; i < affected; ++i)
		{
			if(i) result += ",";
			result += db->Get(i, "rating");
		}
	}

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

string GetMessageCommentsCount(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_comment` where `type`=\"message\" and `messageID`='" + messageID + "';", db);
}

string GetCompanyCommentsCount(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_comment` where `type`=\"company\" and `messageID`='" + messageID + "';", db);
}

string GetLanguageCommentsCount(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_comment` where `type`=\"language\" and `messageID`='" + messageID + "';", db);
}

string GetBookCommentsCount(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_comment` where `type`=\"book\" and `messageID`='" + messageID + "';", db);
}

string GetCertificateCommentsCount(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_comment` where `type` in (\"certification\", \"course\") and `messageID`='" + messageID + "';", db);
}

string GetUniversityDegreeCommentsCount(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_comment` where `type`=\"university\" and `messageID`='" + messageID + "';", db);
}

string GetMessageSpam(string messageID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_params` where `parameter`='spam' and messageID='" + messageID + "';", db);
}

// --- Function returns true or false depends on userID "spamed" it or not
// --- input: messageID, userID
// --- output: was this message "spamed" by particular user or not
string GetMessageSpamUser(string messageID, string userID, CMysql *db)
{
	return GetValueFromDB("select count(*) as `counter` from `feed_message_params` where `parameter`='spam' and `messageID`='" + messageID + "' and `userID`='" + userID + "' ;", db);
}

string UpdateMessageDst(const string &messageID, const string &dstType_new, const string &dstID_new, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;
	auto	affected = db->Query("SELECT * FROM `feed` WHERE `actionTypeId`=\"11\" AND `actionId`=" + quoted(messageID) + ";");

	if(affected)
	{
		auto	srcType_old	= db->Get(0, "srcType");
		auto	userId_old	= db->Get(0, "userId");
		auto	dstType_old	= db->Get(0, "dstType");
		auto	dstID_old	= db->Get(0, "dstID");

		if((dstType_old == dstType_new) && (dstID_old == dstID_new))
		{
			MESSAGE_DEBUG("", "", "nothing to change");
		}
		else if(srcType_old != "user")
		{
			error_message = gettext("to change message destination, it must be written from user only");
			MESSAGE_ERROR("", "", error_message);
		}
		else
		{
			// --- message dst could be changed only if message has been written by user only (not company)
			if(dstType_new == "group")
			{
				if(dstID_new.length())
				{
					auto group_owner_id = GetValueFromDB(Get_OwnerUserIDByGroupID_sqlquery(dstID_new), db);

					if(
						(group_owner_id == user->GetID())
						||
						amISubscribedToGroup(dstID_new, db, user)
						)
					{
						// --- move message to my group or subscribed groups
						db->Query(
							"UPDATE `feed` SET "
							"`dstType`=\"" + dstType_new + "\", "
							"`dstID`=\"" + dstID_new + "\" "
							"WHERE `actionTypeId`=\"11\" AND `actionId`=" + quoted(messageID) + ";"
							);
					}
					else
					{
						error_message = gettext("you are not authorized");
						MESSAGE_ERROR("", "", error_message);
					}
				}
				else
				{
					error_message = gettext("destination group id unknown");
					MESSAGE_ERROR("", "", error_message);
				}
			}
			else if(dstType_new == "")
			{
				// --- move message to users feed
				db->Query(
						"UPDATE `feed` SET "
						"`dstType`=\"" + dstType_new + "\", "
						"`dstID`=\"" + dstID_new + "\" "
						"WHERE `actionTypeId`=\"11\" AND `actionId`=" + quoted(messageID) + ";"
					);
			}
			else
			{
				error_message = gettext("destination type unknown");
				MESSAGE_ERROR("", "", error_message);
			}
		}
	}
	else
	{
		error_message = gettext("message not found");
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

auto amISubscribedToGroup(string group_id, CMysql *db, CUser *user) -> bool
{
	MESSAGE_DEBUG("", "", "start");

	auto result = false;

	if(user && db)
	{
		result = db->Query("SELECT `id` FROM `users_subscriptions` WHERE `entity_type`=\"group\" AND `entity_id`=\"" + group_id + "\" AND `user_id`=\"" + user->GetID() + "\";");
	}
	else
	{
		MESSAGE_ERROR("", "", "user or db parameter is null");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto GetUserRibbons_InJSONFormat(const string &query, CMysql *db) -> string
{
	struct ItemClass 
	{
		string	id;
		string	user_id;
		string	ribbon_id;
		string	received_timestamp;
		string	condition_title;
	};

	vector<ItemClass>		itemList;

	MESSAGE_DEBUG("", "", "start");

	auto	result = ""s;

	if(db)
	{
		if(query.length())
		{
			auto	affected = db->Query(query);

			for(auto i = 0; i < affected; ++i)
			{
				ItemClass	item;

				item.id = db->Get(i, "id");
				item.user_id = db->Get(i, "user_id");
				item.ribbon_id = db->Get(i, "ribbon_id");
				item.received_timestamp = db->Get(i, "received_timestamp");

				itemList.push_back(item);
			}


			for(auto i = 0u; i < itemList.size(); i++)
			{
					if(result.length()) result += ", ";

					result += "{";
					result += "\"id\": \""				  	+ itemList[i].id + "\",";
					result += "\"user_id\": \""				+ itemList[i].user_id + "\",";
					result += "\"ribbon\":"					+ GetRibbons_InJSONFormat("SELECT * FROM `ribbons` WHERE `id` IN (" + itemList[i].ribbon_id + ");", db) + ",";
					result += "\"received_timestamp\": \""	+ itemList[i].received_timestamp + "\"";
					result += "}";
			}
		}
		else
		{
			MESSAGE_ERROR("", "", gettext("mandatory parameter missed"));
		}
	}
	else
	{
		MESSAGE_ERROR("", "", gettext("SQL syntax error"));
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto GetRibbons_InJSONFormat(const string &query, CMysql *db) -> string
{
	struct ItemClass 
	{
		string	id;
		string	title;
		string	image;
		string	display_period_start;
		string	display_period_end;
	};

	vector<ItemClass>		itemList;

	MESSAGE_DEBUG("", "", "start");

	auto	result = ""s;

	if(db)
	{
		if(query.length())
		{
			auto	affected = db->Query(query);

			for(auto i = 0; i < affected; ++i)
			{
				ItemClass	item;

				item.id = db->Get(i, "id");
				item.title = db->Get(i, "title");
				item.image = db->Get(i, "image");
				item.display_period_start = db->Get(i, "display_period_start");
				item.display_period_end = db->Get(i, "display_period_end");

				itemList.push_back(item);
			}


			for(auto i = 0u; i < itemList.size(); i++)
			{
					if(result.length()) result += ", ";

					result += "{";
					result += "\"id\": \""				  	+ itemList[i].id + "\",";
					result += "\"title\": \""				+ itemList[i].title + "\",";
					result += "\"image\": \""				+ RIBBON_URL_FOLDER + itemList[i].image + "\",";
					result += "\"display_period_start\": \""+ itemList[i].display_period_start + "\",";
					result += "\"display_period_end\": \""	+ itemList[i].display_period_end + "\"";
					result += "}";
			}

		}
		else
		{
			MESSAGE_ERROR("", "", gettext("mandatory parameter missed"));
		}
	}
	else
	{
		MESSAGE_ERROR("", "", gettext("SQL syntax error"));
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

static auto __AreActiveRibbons(const string &message_title, CMysql *db, CUser *user)
{
	return GetValuesFromDB(	Get_ActiveRibbonsIDToday_sqlquery() + 
							" AND "
							"(`id` NOT IN(" + Get_UserRibbonsIDByUserID_sqlquery(user->GetID()) + "))"
							" AND "
							"LOCATE(`condition_title`, \"" + message_title + "\")>0"
							, db);
}

static auto __PinRibbonsToAvatar(const vector<string> &ribbons, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;

	for(auto &ribbon_id: ribbons)
	{
		db->InsertQuery("INSERT INTO `users_ribbons` (`user_id`, `ribbon_id`, `received_timestamp`) VALUES (" + user->GetID() + ", " + ribbon_id + ", UNIX_TIMESTAMP());");
	}


	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

auto AttachRibbon(const string &feed_id, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;
	auto	message_title = GetValueFromDB("SELECT `title` FROM `feed_message` WHERE `id` IN (" + Get_MessageIDByFeedID_sqlquery(feed_id) + ");", db);

	if(message_title.length())
	{
		auto	ribbons = __AreActiveRibbons(message_title, db, user);

		if(ribbons.size())
		{
			error_message = __PinRibbonsToAvatar(ribbons, db, user);
		}
		else
		{
			MESSAGE_DEBUG("", "", "no ribbons to pin");
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "message title is empty");
	}

	MESSAGE_DEBUG("", "", "finish (error_message: " + error_message + ")");

	return error_message;
}