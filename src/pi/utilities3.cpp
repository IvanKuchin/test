#include "utilities.h"

// --- input: 
//		dbQuery - example "SELECT * FROM `open_vacancies`;"
//		isAdminUser:
//			true - candidates count will be included
//			false - candidates count will NOT be included
//		db - pointer to CMysql object
//		user - pointer to user (if admin_user - return correct answers)
string GetOpenVacanciesInJSONFormat(string dbQuery, CMysql *db, CUser *user/* = NULL*/)
{
	struct ItemClass 
	{
		string id;
		string company_id;
		string company_position_id;
		string geo_locality_id;
		string salary_min;
		string salary_max;
		string start_month;
		string work_format;
		string description;
		string question1;
		string question2;
		string question3;
		string correct_answer1;
		string correct_answer2;
		string correct_answer3;
		string answer11;
		string answer12;
		string answer13;
		string answer21;
		string answer22;
		string answer23;
		string answer31;
		string answer32;
		string answer33;
		string language1_id;
		string language2_id;
		string language3_id;
		string skill1_id;
		string skill2_id;
		string skill3_id;
		string publish_finish;
		string publish_period;
	};

	vector<ItemClass>		itemsList;
	ostringstream   		ostResult;
	string		 			result = "";
	int						itemsCount = 0;

	MESSAGE_DEBUG("", "", "start");

	ostResult.str("");

	itemsCount = db->Query(dbQuery);
	for(int i = 0; i < itemsCount; ++i)
	{
		ItemClass   item;

		item.id = db->Get(i, "id");
		item.company_id = db->Get(i, "company_id");
		item.company_position_id = db->Get(i, "company_position_id");
		item.geo_locality_id = db->Get(i, "geo_locality_id");
		item.salary_min = db->Get(i, "salary_min");
		item.salary_max = db->Get(i, "salary_max");
		item.start_month = db->Get(i, "start_month");
		item.work_format = db->Get(i, "work_format");
		item.description = db->Get(i, "description");
		item.language1_id = db->Get(i, "language1_id");
		item.language2_id = db->Get(i, "language2_id");
		item.language3_id = db->Get(i, "language3_id");
		item.skill1_id = db->Get(i, "skill1_id");
		item.skill2_id = db->Get(i, "skill2_id");
		item.skill3_id = db->Get(i, "skill3_id");
		item.publish_finish = db->Get(i, "publish_finish");
		item.publish_period = db->Get(i, "publish_period");
		item.question1 = db->Get(i, "question1");
		item.question2 = db->Get(i, "question2");
		item.question3 = db->Get(i, "question3");
		item.correct_answer1 = db->Get(i, "correct_answer1");
		item.correct_answer2 = db->Get(i, "correct_answer2");
		item.correct_answer3 = db->Get(i, "correct_answer3");
		item.answer11 = db->Get(i, "answer11");
		item.answer12 = db->Get(i, "answer12");
		item.answer13 = db->Get(i, "answer13");
		item.answer21 = db->Get(i, "answer21");
		item.answer22 = db->Get(i, "answer22");
		item.answer23 = db->Get(i, "answer23");
		item.answer31 = db->Get(i, "answer31");
		item.answer32 = db->Get(i, "answer32");
		item.answer33 = db->Get(i, "answer33");

		itemsList.push_back(item);
	}

	for(int i = 0; i < itemsCount; ++i)
	{
		string		company_position_title = "";
		string		geo_locality_title = "";
		string		geo_region_id = "";
		string		geo_region_title = "";
		string		language1_title = "";
		string		language2_title = "";
		string		language3_title = "";
		string		skill1_title = "";
		string		skill2_title = "";
		string		skill3_title = "";
		string		number_of_applied_candidates = "";
		string		admin_user_id = "";

		if(db->Query("SELECT * FROM `company_position` WHERE `id`=\"" + itemsList[i].company_position_id + "\";"))
			company_position_title = db->Get(0, "title");
		else
		{
			MESSAGE_ERROR("", "", "ERROR: fail getting company_position by id");
		}

		if(db->Query("SELECT * FROM `geo_locality` WHERE `id`=\"" + itemsList[i].geo_locality_id + "\";"))
		{
			geo_locality_title = db->Get(0, "title");
			geo_region_id = db->Get(0, "geo_region_id");
			if(db->Query("SELECT * FROM `geo_region` WHERE `id`=\"" + geo_region_id + "\";"))
			{
				geo_region_title = db->Get(0, "title");
			}
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting geo_region by id");
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "ERROR: fail getting geo_locality by id");
		}

		if(itemsList[i].language1_id != "0")
		{
			if(db->Query("SELECT * FROM `language` WHERE `id`=\"" + itemsList[i].language1_id + "\";"))
				language1_title = db->Get(0, "title");
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting language1 by id");
			}
		}

		if(itemsList[i].language2_id != "0")
		{
			if(db->Query("SELECT * FROM `language` WHERE `id`=\"" + itemsList[i].language2_id + "\";"))
				language2_title = db->Get(0, "title");
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting language2 by id");
			}
		}

		if(itemsList[i].language3_id != "0")
		{
			if(db->Query("SELECT * FROM `language` WHERE `id`=\"" + itemsList[i].language3_id + "\";"))
				language3_title = db->Get(0, "title");
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting language3 by id");
			}
		}

		if(itemsList[i].skill1_id != "0")
		{
			if(db->Query("SELECT * FROM `skill` WHERE `id`=\"" + itemsList[i].skill1_id + "\";"))
				skill1_title = db->Get(0, "title");
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting skill1 by id");
			}
		}

		if(itemsList[i].skill2_id != "0")
		{
			if(db->Query("SELECT * FROM `skill` WHERE `id`=\"" + itemsList[i].skill2_id + "\";"))
				skill2_title = db->Get(0, "title");
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting skill2 by id");
			}
		}

		if(itemsList[i].skill3_id != "0")
		{
			if(db->Query("SELECT * FROM `skill` WHERE `id`=\"" + itemsList[i].skill3_id + "\";"))
				skill3_title = db->Get(0, "title");
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail getting skill3 by id");
			}
		}

		if(db->Query("SELECT COUNT(*) as `count` FROM `company_candidates` WHERE vacancy_id=\"" + itemsList[i].id + "\" AND `status`=\"applied\";"))
			number_of_applied_candidates = db->Get(0, "count");
		else
			number_of_applied_candidates = "0";

		if(db->Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + itemsList[i].company_id + "\";"))
			admin_user_id = db->Get(0, "admin_userID");

		if(ostResult.str().length()) ostResult << ", ";

		ostResult	<< "{"
					<< "\"id\":\"" << itemsList[i].id << "\","
					<< "\"company_id\":\"" << itemsList[i].company_id << "\","
					// << "\"company\":" << GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id`=\"" + itemsList[i].company_id + "\";", db, NULL) << ","
					<< "\"company_position_id\":\"" << itemsList[i].company_position_id << "\","
					<< "\"company_position_title\":\"" << company_position_title << "\","
					<< "\"geo_locality_id\":\"" << itemsList[i].geo_locality_id << "\","
					<< "\"geo_locality_title\":\"" << geo_locality_title << "\","
					<< "\"geo_region_id\":\"" << geo_region_id << "\","
					<< "\"geo_region_title\":\"" << geo_region_title << "\","
					<< "\"salary_min\":\"" << itemsList[i].salary_min << "\","
					<< "\"salary_max\":\"" << itemsList[i].salary_max << "\","
					<< "\"start_month\":\"" << itemsList[i].start_month << "\","
					<< "\"work_format\":\"" << itemsList[i].work_format << "\","
					<< "\"description\":\"" << itemsList[i].description << "\","
					<< "\"question1\":\"" << itemsList[i].question1 << "\","
					<< "\"question2\":\"" << itemsList[i].question2 << "\","
					<< "\"question3\":\"" << itemsList[i].question3 << "\","
					<< "\"correct_answer1\":\"" << (user && (admin_user_id == user->GetID()) ? itemsList[i].correct_answer1 : "") << "\","
					<< "\"correct_answer2\":\"" << (user && (admin_user_id == user->GetID()) ? itemsList[i].correct_answer2 : "") << "\","
					<< "\"correct_answer3\":\"" << (user && (admin_user_id == user->GetID()) ? itemsList[i].correct_answer3 : "") << "\","
					<< "\"answer11\":\"" << itemsList[i].answer11 << "\","
					<< "\"answer12\":\"" << itemsList[i].answer12 << "\","
					<< "\"answer13\":\"" << itemsList[i].answer13 << "\","
					<< "\"answer21\":\"" << itemsList[i].answer21 << "\","
					<< "\"answer22\":\"" << itemsList[i].answer22 << "\","
					<< "\"answer23\":\"" << itemsList[i].answer23 << "\","
					<< "\"answer31\":\"" << itemsList[i].answer31 << "\","
					<< "\"answer32\":\"" << itemsList[i].answer32 << "\","
					<< "\"answer33\":\"" << itemsList[i].answer33 << "\","
					<< "\"language1_id\":\"" << itemsList[i].language1_id << "\","
					<< "\"language1_title\":\"" << language1_title << "\","
					<< "\"language2_id\":\"" << itemsList[i].language2_id << "\","
					<< "\"language2_title\":\"" << language2_title << "\","
					<< "\"language3_id\":\"" << itemsList[i].language3_id << "\","
					<< "\"language3_title\":\"" << language3_title << "\","
					<< "\"skill1_id\":\"" << itemsList[i].skill1_id << "\","
					<< "\"skill1_title\":\"" << skill1_title << "\","
					<< "\"skill2_id\":\"" << itemsList[i].skill2_id << "\","
					<< "\"skill2_title\":\"" << skill2_title << "\","
					<< "\"skill3_id\":\"" << itemsList[i].skill3_id << "\","
					<< "\"skill3_title\":\"" << skill3_title << "\","
					<< "\"publish_finish\":\"" << itemsList[i].publish_finish << "\","
					<< "\"publish_period\":\"" << itemsList[i].publish_period << "\","
					<< "\"number_of_applied_candidates\":\"" << (user && (admin_user_id == user->GetID()) ? number_of_applied_candidates : "") << "\""
					<<"}";
	}

	result = ostResult.str();

	MESSAGE_DEBUG("", "", "end (returning string length " + to_string(result.length()) + ")");

	return result;
}
