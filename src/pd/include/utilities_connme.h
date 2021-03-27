#ifndef __UTILITIES_CONNME__H__
#define __UTILITIES_CONNME__H__

#include "c_cache_obj.h"
#include "utilities.h"
#include "utilities_sql_queries.h"


// --- User language expertise functions
// ------- do NOT yse this function
// ------- it is used ONLY for admin part
auto 			GetLanguageListInJSONFormat(string dbQuery, CMysql *, bool includeStudents = false) -> string;
// ------- use this version, if possible
auto 			GetUserLanguageListInJSONFormat(string user_id, CMysql *) -> string;


// --- this function returns lots of data, be careful with it
auto 	     	GetUserListInJSONFormat(string dbQuery, CMysql *, CUser *) -> string;
// --- use short version of previous function, where possible
auto 	     	GetUsersNameAvatarInJSONFormat(string dbQuery, CMysql *, CUser *) -> string;

auto 			GetBookListInJSONFormat(string dbQuery, CMysql *, bool includeReaders = false) -> string;
auto 			GetComplainListInJSONFormat(string dbQuery, CMysql *, bool includeReaders = false) -> string;
auto 			GetCertificationListInJSONFormat(string dbQuery, CMysql *, bool includeDevoted = false) -> string;
auto 			GetCourseListInJSONFormat(string dbQuery, CMysql *, bool includeStudents = false) -> string;
auto 			GetSkillListInJSONFormat(string user_id, CMysql *) -> string;
auto 			GetUniversityListInJSONFormat(string dbQuery, CMysql *, bool includeStudents = false) -> string;
auto 			GetSchoolListInJSONFormat(string dbQuery, CMysql *, bool includeStudents = false) -> string;
auto			GetUserRibbons_InJSONFormat(const string &query, CMysql *db) -> string;
auto 			GetRibbons_InJSONFormat(const string &query, CMysql *db) -> string;
auto 			AttachRibbon(const string &feed_id, CMysql *db, CUser *user) -> string;


auto			GetCompanyPositionIDByTitle(string title, CMysql *) -> string;
auto			GetLanguageIDByTitle(string title, CMysql *) -> string;
auto			GetSkillIDByTitle(string title, CMysql *) -> string;
auto 			GetBookRatingList(string bookID, CMysql *) -> string;
auto 			GetCourseRatingList(string courseID, CMysql *) -> string;
auto 	     	GetMessageCommentsCount(string messageID, CMysql *) -> string;
auto 			GetCompanyCommentsCount(string messageID, CMysql *) -> string;
auto 			GetLanguageCommentsCount(string messageID, CMysql *) -> string;
auto 			GetBookCommentsCount(string messageID, CMysql *) -> string;
auto 			GetCertificateCommentsCount(string messageID, CMysql *) -> string;
auto 			GetUniversityDegreeCommentsCount(string messageID, CMysql *) -> string;
auto    	  	GetMessageSpam(string messageID, CMysql *) -> string;
auto    	  	GetMessageSpamUser(string messageID, string userID, CMysql *) -> string;
auto			UpdateMessageDst(const string &messageID, const string &dstType_new, const string &dstID_new, CMysql *, CUser *) -> string;
auto			amISubscribedToGroup(string group_id, CMysql *, CUser *) -> bool;

auto	      	GetMessageImageList(string imageSetID, CMysql *) -> string;
auto    	  	GetCompanyListInJSONFormat(string dbQuery, CMysql *, CUser *, bool quickSearch = true, bool includeEmployedUsersList = false) -> string;
auto 			GetNewsFeedInJSONFormat(string whereStatement, int currPage, int newsOnSinglePage, CUser *, CMysql *) -> string;
auto      		GetMessageLikesUsersList(string messageID, CUser *, CMysql *) -> string;
auto 			GetBookLikesUsersList(string usersBookID, CUser *, CMysql *) -> string;
auto 			GetLanguageLikesUsersList(string usersLanguageID, CUser *, CMysql *) -> string;
auto 			GetCompanyLikesUsersList(string usersCompanyID, CUser *, CMysql *) -> string;
auto 			GetCertificationLikesUsersList(string usersCertificationID, CUser *, CMysql *) -> string;
auto 			GetCourseLikesUsersList(string usersCourseID, CUser *, CMysql *) -> string;
auto 			GetUniversityDegreeLikesUsersList(string messageID, CUser *, CMysql *) -> string;
auto 			GetBookRatingUsersList(string bookID, CUser *, CMysql *) -> string;
auto 	     	GetUserNotificationSpecificDataByType(unsigned long typeID, unsigned long actionID, CMysql *, CUser *) -> string;
auto    	  	GetUserNotificationInJSONFormat(string sqlRequest, CMysql *, CUser *) -> string;
auto 			GetCandidatesListAppliedToVacancyInJSONFormat(string dbQuery, CMysql *) -> string;

auto			RotateImage(string filename, int degree) -> string;
auto			FlipImageVertical(string filename) -> string;
auto			FlipImageHorizontal(string filename) -> string;
auto			RenameImageInDB(string image_id, CMysql *) -> string;

auto			GetUserListInJSONFormat_BySearchString(const string &lookForKey, bool include_myself, CMysql *, CUser *) -> string;
auto			GetUsersID_BySearchString(const string &lookForKey, bool include_myself, CMysql *db, CUser *user) -> vector<string>;

auto      		GetGroupListInJSONFormat(string dbQuery, CMysql *, CUser *) -> string;

auto	      	GetUnreadChatMessagesInJSONFormat(CUser *, CMysql *) -> string;

#endif
