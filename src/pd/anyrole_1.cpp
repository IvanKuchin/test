#include "anyrole.h"

static auto RenumberOrder_in_ImageSet(const string &set_id, CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	auto							error_message = ""s;
	vector<pair<string, string>>	images;
	auto							affected = db->Query("SELECT `id`, `order` FROM `feed_images` WHERE `setID`=" + quoted(set_id) + " ORDER BY `order` ASC;");

	for(auto i = 0; i < affected; ++i)
		images.push_back(make_pair(db->Get(i, "id"), db->Get(i, "order")));

	for(auto i = 0; i < affected; ++i)
		db->Query("UPDATE `feed_images` SET `order`=" + quoted(to_string(i)) + " WHERE `id`=" + images[i].first + ";");

	MESSAGE_DEBUG("", "", "finish(" + error_message + ")");

	return error_message;
}

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	c_config		config(CONFIG_DIR);
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	MESSAGE_DEBUG("", action, __FILE__);

	signal(SIGSEGV, crash_handler);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);    /* Flawfinder: ignore */

	try
	{

		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect(&config) < 0)
		{
			MESSAGE_ERROR("", action, "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));
		{
			MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);
		}

		// --- generate common parts
		{
			// --- it has to be run before session, because session relay upon Apache environment variable
			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			//------- Generate session
			action = GenerateSession(action, &config, &indexPage, &db, &user);
			action = LogoutIfGuest(action, &config, &indexPage, &db, &user);

		}

#ifndef IMAGEMAGICK_DISABLE
		Magick::InitializeMagick(NULL);
#endif

	// ------------ end generate common parts

		MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");

		if((action.length() > 10) && (action.compare(action.length() - 9, 9, "_template") == 0))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			{
				string		template_name = action.substr(0, action.length() - 9) + ".htmlt";

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_ERROR("", action, "can't find template " + template_name);
				} // if(!indexPage.SetTemplate("my_network.htmlt"))
			}

			{
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "AJAX_swapMessageImages")
		{
			auto			id1 = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id1"));
			auto			id2 = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id2"));
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(id1.length() && id2.length())
			{
				if(db.Query(Get_UserIDByImageID_sqlquery(id1)))
				{
					auto	owner_id = db.Get(0, 0);

					if(owner_id == user.GetID())
					{
						if(db.Query(Get_SetIDByImageID_sqlquery(id1)))
						{
							auto	set_id1 = db.Get(0, 0);

							if(db.Query(Get_SetIDByImageID_sqlquery(id2)))
							{
								auto	set_id2 = db.Get(0, 0);

								if(set_id1 == set_id2)
								{
									if(db.Query(Get_OrderByImageID_sqlquery(id1)))
									{
										auto	order1 = db.Get(0, 0);

										if(db.Query(Get_OrderByImageID_sqlquery(id2)))
										{
											auto	order2 = db.Get(0, 0);

											db.Query("UPDATE `feed_images` SET `order`=" + quoted(order1) + " WHERE `id`=(" + id2 + ");");
											db.Query("UPDATE `feed_images` SET `order`=" + quoted(order2) + " WHERE `id`=(" + id1 + ");");
										}
										else
										{
											error_message = gettext("Access prohibited");
											MESSAGE_DEBUG("", "", error_message);
										}
									}
									else
									{
										error_message = gettext("Access prohibited");
										MESSAGE_DEBUG("", "", error_message);
									}

								}
								else
								{
									error_message = gettext("Access prohibited");
									MESSAGE_DEBUG("", "", error_message);
								}
							}
							else
							{
								error_message = gettext("Access prohibited");
								MESSAGE_DEBUG("", "", error_message);
							}
						}
						else
						{
							error_message = gettext("Access prohibited");
							MESSAGE_DEBUG("", "", error_message);
						}

					}
					else
					{
						error_message = gettext("Access prohibited");
						MESSAGE_DEBUG("", "", error_message);
					}
				}
				else
				{
					error_message = gettext("user not found");
					MESSAGE_DEBUG("", "", error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", "", error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		if(action == "AJAX_changeMessageImages_AllTheWayUp")
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(id.length())
			{
				if(db.Query(Get_UserIDByImageID_sqlquery(id)))
				{
					auto	owner_id = db.Get(0, 0);

					if(owner_id == user.GetID())
					{
						if(db.Query(Get_SetIDByImageID_sqlquery(id)))
						{
							auto	set_id = db.Get(0, 0);

							db.Query("UPDATE `feed_images` SET `order`=`order`+1 WHERE `setID`=(" + quoted(set_id) + ");");
							db.Query("UPDATE `feed_images` SET `order`=0 WHERE `id`=(" + id + ");");

							if(db.Query(Get_SetIDByImageID_sqlquery(id)))
							{
								auto 	set_id = db.Get(0, 0);

								error_message = RenumberOrder_in_ImageSet(set_id, &db);
							}
							else
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_DEBUG("", "", error_message);
							}
						}
						else
						{
							error_message = gettext("Access prohibited");
							MESSAGE_DEBUG("", "", error_message);
						}

					}
					else
					{
						error_message = gettext("Access prohibited");
						MESSAGE_DEBUG("", "", error_message);
					}
				}
				else
				{
					error_message = gettext("user not found");
					MESSAGE_DEBUG("", "", error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", "", error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		if(action == "AJAX_changeMessageImages_AllTheWayDown")
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(id.length())
			{
				if(db.Query(Get_UserIDByImageID_sqlquery(id)))
				{
					auto	owner_id = db.Get(0, 0);

					if(owner_id == user.GetID())
					{
						if(db.Query(Get_MaxOrderByImageID_sqlquery(id)))
						{
							auto	max_order = db.Get(0, 0);

							db.Query("UPDATE `feed_images` SET `order`=(" + max_order + " + 1) WHERE `id`=(" + id + ");");


							if(db.Query(Get_SetIDByImageID_sqlquery(id)))
							{
								auto 	set_id = db.Get(0, 0);

								error_message = RenumberOrder_in_ImageSet(set_id, &db);
							}
							else
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_DEBUG("", "", error_message);
							}

						}
						else
						{
							error_message = gettext("Access prohibited");
							MESSAGE_DEBUG("", "", error_message);
						}

					}
					else
					{
						error_message = gettext("Access prohibited");
						MESSAGE_DEBUG("", "", error_message);
					}
				}
				else
				{
					error_message = gettext("user not found");
					MESSAGE_DEBUG("", "", error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", "", error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		if(
			(action == "AJAX_rotateImageCounterclockwise")
			||
			(action == "AJAX_rotateImageClockwise")
			||
			(action == "AJAX_flipImageVertical")
			||
			(action == "AJAX_flipImageHorizontal")
		)
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(id.length())
			{
				if(db.Query(Get_MessageIDByImageID_sqlquery(id)))
				{
					auto	message_id = db.Get(0, 0);

					if(AmIMessageOwner(message_id, &user, &db))
					{
						if(db.Query("SELECT `folder`, `filename` FROM `feed_images` WHERE `id`=" + quoted(id) + ";"))
						{
							auto	filename = IMAGE_FEED_DIRECTORY + FS_SEPARATOR + db.Get(0, "folder") + FS_SEPARATOR + db.Get(0, "filename");

							if(action == "AJAX_rotateImageCounterclockwise") 	error_message = RotateImage(filename, -90);
							if(action == "AJAX_rotateImageClockwise") 			error_message = RotateImage(filename, 90);
							if(action == "AJAX_flipImageHorizontal") 			error_message = FlipImageHorizontal(filename);
							if(action == "AJAX_flipImageVertical") 				error_message = FlipImageVertical(filename);
							if(error_message.empty())
							{
								error_message = RenameImageInDB(id, &db);
							}
							else
							{
								MESSAGE_DEBUG("", "", error_message);
							}
						}
						else
						{
							error_message = gettext("image not found");
							MESSAGE_DEBUG("", "", error_message);
						}
					}
					else
					{
						error_message = gettext("Access prohibited");
						MESSAGE_DEBUG("", "", error_message);
					}
				}
				else
				{
					error_message = gettext("message not found");
					MESSAGE_DEBUG("", "", error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", "", error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		if(action == "AJAX_getUserAutocompleteList")
		{
			auto			term = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("term"));
			auto			template_name = "json_response.htmlt";
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(term.length())
			{
				auto	users_id	= GetUsersID_BySearchString(term, false, &db, &user);

				if(users_id.size())
				{
					int		affected	= db.Query("SELECT `id`, `name`, `nameLast` FROM `users` WHERE `id` IN (" + join(users_id, ",") + ") LIMIT 0, 20;");

					for(int i = 0; i < affected; ++i)
					{
						if(i) success_message += ",";
						success_message += "{\"id\":\"" + db.Get(i, "id") + "\",\"label\":\"" + RemoveQuotas(ConvertHTMLToText(db.Get(i, "name") + " " + db.Get(i, "nameLast"))) + "\"}";
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "no users found");
				}
			}
			else
			{
				error_message = gettext("user id not found");
				MESSAGE_DEBUG("", action, error_message);
			}


			if(error_message.empty())
			{
				indexPage.RegisterVariableForce("result", "[" + success_message + "]");
			}
			else
			{
				MESSAGE_DEBUG("", action, error_message);
	
				indexPage.RegisterVariableForce("result", "[]");
			}

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}

		if(action == "AJAX_getGroupsOwnedByUser")
		{
			auto			success_message = "\"groups\":[" + GetGroupListInJSONFormat("SELECT * FROM `groups` WHERE `owner_id`=" + quoted(user.GetID()) + ";", &db, &user) + "]";
			auto			error_message = ""s;

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		if(action == "AJAX_getGroupsOwnedByUserAndSubscribedTo")
		{
			auto			success_message = "\"groups\":[" + GetGroupListInJSONFormat("SELECT * FROM `groups` WHERE `owner_id`=" + quoted(user.GetID()) + " OR `id` IN (" + Get_Groups_UserSubscribedTo_sqlquery(user.GetID()) + ");", &db, &user) + "]";
			auto			error_message = ""s;

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		MESSAGE_DEBUG("", "", "post-condition if(action == \"" + action + "\")");

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);
		MESSAGE_DEBUG("", action, "catch CExceptionHTML: DEBUG exception reason: [" + c.GetReason() + "]");

		if(!indexPage.SetTemplate(c.GetTemplate()))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(0);
	}
	catch(CException &c)
	{
		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		{
			MESSAGE_ERROR("", action, "catch CException: exception: ERROR  " + c.GetReason());
		}

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		{
			MESSAGE_ERROR("", action, "catch(exception& e): catch standard exception: ERROR  " + e.what());
		}

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", e.what());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}
