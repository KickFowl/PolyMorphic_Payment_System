#include <conio.h>
#include <iostream>
#include <string>
#include <ranges>
#include <Windows.h>

#define TM [char]0x2122l;

enum cardProviders {
	VISA, MASTERCARD, DISCOVER, AMARICAN_EXPRESS
};

void backspace();
void updateStatusLine(const std::string& message);

class PaymentMethod
{

public:

	virtual void processPayment()
	{
		std::cout << "Memory overflow error: Maybe this wouldn't have happened if you had paid properly.\n";
	}

	virtual ~PaymentMethod() = default;
};

class DebitCard : public PaymentMethod
{
	cardProviders cardProvider;

public:

	void processPayment()
	{
		std::cout << "Starting card collection\n\n";

		bool isGood = checkCardNumber();

		if (isGood)
		{
			std::cout << "\n\nGood boy, you can pay for stuff ;)";
		}
		else std::cout << "\n\nHow???";
	}

	bool checkCardNumber()
	{
		std::cout << "Enter card number: ";

		bool noError = true;

		char ch;
		std::string cardNumber = "";
		int charCount = 0;

		while (true)
		{
			ch = _getch();

			switch (ch)
			{
			case 8:
				if (charCount > 0)
				{
					cardNumber.pop_back();

					backspace();

					if (charCount == 1)
					{
						noError = true;
						//updateStatusLine("");
					}

					if (cardProvider == AMARICAN_EXPRESS)
					{
						if (charCount == 4 || charCount == 10)
						{
							backspace();
						}
						if (charCount == 13)
						{
							noError = true;
							//updateStatusLine("");
						}
					}
					else
					{
						if (charCount == 4 || charCount == 8 || charCount == 12)
						{
							backspace();
						}
						if (charCount == 15)
						{
							noError = true;
							//updateStatusLine("");
						}
					}

					charCount--;
				}
				updateStatusLine(std::to_string(charCount) + " : 1 : " + cardNumber);
				continue;
			case 27:
				return 0;
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 58:
				if (!noError)
				{
					updateStatusLine(std::to_string(charCount) + " : 2 : " + cardNumber);
					continue;
				}
				std::cout << ch;

				if (cardNumber == "")
				{
					cardNumber += ch;
					break;
				}
				cardNumber += ch;
				break;
			default:
				updateStatusLine(std::to_string(charCount) + " : 3 : " + cardNumber);
				continue;
			}

			if (charCount == 1)
			{
				if (cardNumber == "60" || cardNumber == "64" || cardNumber == "65") cardProvider = DISCOVER;
				else if (std::stoi(cardNumber) >= 22 && std::stoi(cardNumber) < 28) cardProvider = MASTERCARD;
				else if (std::stoi(cardNumber) >= 40 && std::stoi(cardNumber) < 50) cardProvider = VISA;
				else if (cardNumber == "34" || cardNumber == "37") cardProvider = AMARICAN_EXPRESS;
				else if (std::stoi(cardNumber) >= 51 && std::stoi(cardNumber) < 56) cardProvider = MASTERCARD;
				else
				{
					charCount++;
					updateStatusLine("ERROR: Invalid card number");
					noError = false;
					//updateStatusLine(std::to_string(charCount) + " : " + std::to_string(cardNumber));
					continue;
				}
			}

			if (cardProvider == AMARICAN_EXPRESS)
			{
				if (charCount == 14)
				{
					if (checkLuhn(cardNumber) && noError)
					{
						break;
					}
					else
					{
						noError = false;
						updateStatusLine("ERROR: Invalid card number");
					}
				}
				if (charCount == 3 || charCount == 9) std::cout << '-';
			}
			else
			{
				if (charCount == 15)
				{
					if (checkLuhn(cardNumber) && noError)
					{
						break;
					}
					else
					{
						noError = false;
						updateStatusLine("ERROR: Invalid card number");
					}
				}
				if (charCount == 3 || charCount == 7 || charCount == 11) std::cout << '-';
			}

			charCount++;
			updateStatusLine(std::to_string(charCount) + " : 4 : " + cardNumber);
		}

		return true;
	}

	bool checkLuhn(int number)
	{
		std::string numString = std::to_string(number);

		bool big = false;
		int check = 0;

		for (char cr : std::views::reverse(numString))
		{
			if (big)
			{
				int num = (cr - '0') * 2;
				if (num > 9) num - 9;
				check += num;

				big = false;
			}
			else
			{
				check += cr - '0';
				big = true;
			}
		}

		if (check % 10 == 0) return true;
		return false;
	}

	bool checkLuhn(std::string& number)
	{
		std::string numString = number;

		bool big = false;
		int check = 0;

		for (char cr : std::views::reverse(numString))
		{
			if (big)
			{
				int num = (cr - '0') * 2;
				if (num > 9) num -= 9;
				check += num;

				big = false;
			}
			else
			{
				check += cr - '0';
				big = true;
			}
		}

		if (check % 10 == 0) return true;
		return false;
	}
};

class TimothyPay : public PaymentMethod
{

	void processPayment()
	{
		std::cout << "Do you promise to pay me back if I cover this? [Y/N]: ";

		std::string contract;

		std::cin >> contract;

		switch (contract[0])
		{
		case 'Y':
			std::cout << "I knew I could trust you!!! :)";
			return;
		case 'y':
			std::cout << "I knew I could trust you!!! :)";
			return;
		case 'N':
			std::cout << "You seem nice enough I just won't take your word for it. Here's some money :)";
			return;
		case 'n':
			std::cout << "You seem nice enough I just won't take your word for it. Here's some money :)";
			return;
		default:
			std::cout << "That's not a valid response...";
			Sleep(2000);
			std::cout << "\nOh well here's some money :)";
		}
	}

};

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	PaymentMethod* paymentProcess;

	std::cout << "How would you like to pay?\n";
	std::cout << "1:Debit/Credit     2:TimothyPrePay™" << std::endl;

	int choice;
	std::cin >> choice;

	switch (choice)
	{
	case 1:
		paymentProcess = new DebitCard();
		paymentProcess->processPayment();
		break;
	case 2:
		paymentProcess = new TimothyPay();
		paymentProcess->processPayment();
		break;
	default:
		std::cout << "ERROR: Invalid number Debit assumed.\n";
		paymentProcess = new DebitCard();
		paymentProcess->processPayment();
		break;
	}
	return 0;
}

void backspace() {
	std::cout << "\033[1D \033[1D" << std::flush;
}

void updateStatusLine(const std::string& message) {
	std::cout << "\033[s"
		<< "\n"          
		<< "\033[2K"     
		<< "\033[31m"    
		<< message       
		<< "\033[0m"     
		<< "\033[u"      
		<< std::flush;   
}