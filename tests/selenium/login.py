import time
from random import random

from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException
from selenium.webdriver.chrome.webdriver import WebDriver
from selenium.webdriver.remote.webelement import WebElement

test_counter_global: int = 1
TEST_LOGIN    = "__test_user__"
TEST_PASSWORD = "__test_password__"


def create_incognito_window():
    """

    :rtype: WebDriver
    """
    chrome_options = webdriver.ChromeOptions()
    chrome_options.add_argument("--incognito")

    browser = webdriver.Chrome("c:\\Users\\ikuchin\\Downloads\\chromedriver.exe", chrome_options = chrome_options)

    return browser;


def close_window(browser: WebDriver) -> object:
    browser.close()


def test_anonymous_feed(browser: WebDriver) -> str:
    """

    :rtype: str
    """
    result: str = ""

    browser.get("https://dev.connme.ru/user/ivan.kuchin")
    time.sleep(1)

    try:
        search_box = browser.find_element_by_id("navMenuSearchText")
    except NoSuchElementException as e:
        result = "fail to find search box"

    return result

def login_to_site(browser: WebDriver, login: str, password: str) -> str:
    """

    :rtype: str
    """
    result: str = ""

    browser.get("https://dev.connme.ru/?random=" + str(random() * 7654345789))
    time.sleep(1)

    try:
        login_input: WebElement = browser.find_element_by_id("signinInputLogin")
        login_input.send_keys(login)

        try:
            pass_input: WebElement = browser.find_element_by_id("signinInputPassword")
            pass_input.send_keys(password)

            try:
                enter_button: WebElement = browser.find_element_by_id("signinSubmit")
                enter_button.click()
                time.sleep(2)

                try:
                    search_box = browser.find_element_by_id("navMenuSearchText")

                except NoSuchElementException as e:
                    result = "fail to find search box"

            except NoSuchElementException as e:
                result = "fail to find pass input"

        except NoSuchElementException as e:
            result = "fail to find pass input"

    except NoSuchElementException as e:
        result = "fail to find login input"

    return result


def test_normal_login(browser: WebDriver, login: str, password: str) -> str:
    """

    :rtype: str
    """
    result: str = login_to_site(browser, login, password)

    if(len(result) == 0):
        browser.get("https://dev.connme.ru/logout?rand=" + str(random() * 843234567))

    return result

def test_cookieless_login(browser: WebDriver, login: str, password: str) -> str:
    """

    :type browser: str
    :rtype: str
    """
    result: str = ""

    result: str = login_to_site(browser, login, password)

    if (len(result) == 0):
        browser.delete_cookie("sessid")

        browser.get("https://dev.connme.ru/?random=" + str(random() * 7654345789))
        time.sleep(2)

        try:
            search_box = browser.find_element_by_id("navMenuSearchText")

        except NoSuchElementException as e:
            result = "fail to find search box"

    return result



def test_error(error_message):
    global test_counter_global

    print("Test " + str(test_counter_global) + ":\tERROR: " + error_message)
    test_counter_global += 1


def test_success(success_message):
    """

    :type success_message: optional string message
    """
    global test_counter_global

    print("Test " + str(test_counter_global) + ":\tSUCCESS " + success_message)
    test_counter_global += 1


if __name__ == "__main__":
    incognito_win = create_incognito_window()

    error_message: str = test_anonymous_feed(incognito_win)
    if len(error_message) != 0:
        test_error(error_message)
    else:
        test_success("test_anonymous_feed")

    error_message: str = test_normal_login(incognito_win, TEST_LOGIN, TEST_PASSWORD)
    if len(error_message) != 0:
        test_error(error_message)
    else:
        test_success("test_normal_login")

    error_message: str = test_cookieless_login(incognito_win, TEST_LOGIN, TEST_PASSWORD)
    if len(error_message) != 0:
        test_error(error_message)
    else:
        test_success("test_cookieless_login")

    close_window(incognito_win)
