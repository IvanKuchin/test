#ifndef __UTILITIES_SQL_QUERIES__H__
#define __UTILITIES_SQL_QUERIES__H__

#include <string>

inline auto Get_UserIDByImageID_sqlquery(const string &id)
{
	return (
		"SELECT `userId` FROM `feed` WHERE `actionTypeId`=\"11\" AND `actionId` IN ("
			"SELECT `id` FROM `feed_message` WHERE `imageSetID` IN ("
				"SELECT `setID` FROM `feed_images` WHERE `id` IN (" + id + ")"
			")"
		") "
		);
}

inline auto Get_UserIDByGroupID_sqlquery(const string &id)
{
	return (
				"SELECT `user_id` FROM `users_subscriptions` WHERE `entity_id` IN (" + id + ") and `entity_type`=\"group\""
		);
}

inline auto Get_OwnerUserIDByGroupID_sqlquery(const string &id)
{
	return (
				"SELECT `owner_id` FROM `groups` WHERE `id` IN (" + id + ")"
		);
}

inline auto Get_Groups_UserSubscribedTo_sqlquery(const string &id)
{
	return (
				"SELECT `entity_id` FROM `users_subscriptions` WHERE `user_id` IN (" + id + ") AND `entity_type`=\"group\""
		);
}

inline auto Get_Companies_UserSubscribedTo_sqlquery(const string &id)
{
	return (
				"SELECT `entity_id` FROM `users_subscriptions` WHERE `user_id` IN (" + id + ") AND `entity_type`=\"company\""
		);
}

inline auto Get_SetIDByImageID_sqlquery(const string &id)
{
	return (
				"SELECT `setID` FROM `feed_images` WHERE `id` IN (" + id + ")"
		);
}

inline auto Get_MessageIDByImageID_sqlquery(const string &id)
{
	return (
				"SELECT `id` FROM `feed_message` WHERE `imageSetID` IN (" + Get_SetIDByImageID_sqlquery(id) + ")"
		);
}

inline auto Get_MessageIDByFeedID_sqlquery(const string &id)
{
	return (
				"SELECT `actionId` FROM `feed` WHERE `id` IN (" + id + ") AND `actionTypeId`=\"11\""
		);
}

inline auto Get_OrderByImageID_sqlquery(const string &id)
{
	return (
				"SELECT `order` FROM `feed_images` WHERE `id` IN (" + id + ")"
		);
}

inline auto Get_MaxOrderByImageID_sqlquery(const string &id)
{
	return (
			"SELECT MAX(`order`) FROM `feed_images` WHERE `setID` IN ("
				"SELECT `setID` FROM `feed_images` WHERE `id` IN (" + id + ")"
			")"
		);
}

inline auto Get_UserRibbonsIDByUserID_sqlquery(const string &id)
{
	return (
				"SELECT `ribbon_id` FROM `users_ribbons` WHERE `user_id` IN (" + id + ")"
		);
}

inline auto Get_RibbonsIDByUserID_DisplayToday_sqlquery(const string &id)
{
	return (
				"SELECT * FROM `ribbons` WHERE `id` IN "
				"(" + Get_UserRibbonsIDByUserID_sqlquery(id) + ")"
				"AND"
				"("
				"	(`ribbons`.`display_period_start` <= DATE(NOW())) AND (DATE(NOW()) <= `ribbons`.`display_period_end`)"
				")"
		);
}

inline auto Get_ActiveRibbonsIDToday_sqlquery()
{
	return (
				"SELECT `id` FROM `ribbons` WHERE "
				"("
				"	(`ribbons`.`receive_period_start` <= DATE(NOW())) AND (DATE(NOW()) <= `ribbons`.`receive_period_end`)"
				")"s
		);
}

#endif
