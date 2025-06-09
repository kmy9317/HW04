#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std; 

class Book 
{
public:
    string title;
    string author;

    Book(const string& title, const string& author)
        : title(title), author(author) 
    {
    }
};

class BookManager 
{
private:
    vector<Book> books; 

public:
    // 책 추가 함수
    void addBook(const string& title, const string& author) 
    {
        books.push_back(Book(title, author)); // push_back 사용
        cout << "책이 추가되었습니다: " << title << " by " << author << endl;
    }

    // 모든 책 출력 함수
    void displayAllBooks() const 
    {
        if (books.empty())
        {
            cout << "현재 등록된 책이 없습니다." << endl;
            return;
        }

        cout << "현재 도서 목록:" << endl;
        for (size_t i = 0; i < books.size(); i++) 
        { 
            cout << "- " << books[i].title << " by " << books[i].author << endl;
        }
    }

    // 제목과 저자 검색을 통합한 템플릿 함수
    template<typename Predicate>
    void searchBooks(const string& searchType, Predicate pred) const 
    {
        if (books.empty()) 
        {
            cout << "현재 등록된 책이 없습니다." << endl;
            return;
        }

        cout << searchType << " 검색 결과:" << endl;

        for (const Book& book : books) 
        {
            if (pred(book))
            {
                cout << "- " << book.title << " by " << book.author << endl;
                return;
            }
        }
        cout << "현재 발견된 책이 없습니다." << endl;   
    }

    void searchByTitle(const string& title) const 
    {
        // 람다 참조 캡쳐 사용
        searchBooks("제목", [&title](const Book& book) 
            {
                return book.title == title;
            });
    }

    void searchByAuthor(const string& author) const 
    {
        searchBooks("저자", [&author](const Book& book) 
            {
                return book.author == author;
            });
    }

    // getBook 함수들을 통합한 템플릿 함수
    template<typename Predicate>
    Book* findBook(Predicate pred) 
    {
        auto it = find_if(books.begin(), books.end(), pred);
        return (it != books.end()) ? &(*it) : nullptr;
    }

    Book* getBookByTitle(const string& title) 
    {
        return findBook([&title](const Book& book) 
            {
                return book.title == title;
            });
    }

    Book* getBookByAuthor(const string& author) 
    {
        return findBook([&author](const Book& book)
            {
                return book.author == author;
            });
    }
};

class BorrowManager
{
private:
    unordered_map<string, int> stock;

public:
    // 책 3권으로 제한
    void initializeStock(const Book& book, int quantity = 3) 
    { 
        stock[book.title] = quantity;
    }
    // 대여
    void borrowBook(const string& title)
    {
        auto it = stock.find(title);
        // 책 자체가 등록이 안된 경우
        if (it == stock.end()) 
        {
            cout << "도서관에 " << title << "이 등록되어 있지 않습니다." << endl;
            return;
        }
        // 책 재고가 남은 경우
        if (it->second > 0) 
        {
            it->second--;
            cout << title << "대여 성공. 남은 재고 : " << it->second << endl;
        }
        // 책 재고가 없는 경우
        else
        {
            cout << title << " 재고가 없습니다." << endl;
        }
    }
    // 반납
    void returnBook(const string& title)
    {
        auto it = stock.find(title);
        if (it == stock.end()) 
        {
            cout << "도서관에 " << title << "이 등록되어 있지 않습니다." << endl;
            return;
        }
        // 책 반납(책 갯수 증가)
        it->second++;
    }
    // 전체 조회
    void displayStock()
    {
        if (stock.empty()) 
        {
            cout << "재고 정보가 없습니다" << endl;
            return;
        }
        cout << "전체 재고: " << endl;
        // 전체 등록된 책과 책이 몇권 남았는지 확인
        for (const auto& s : stock)
        {
            cout << "- " << s.first << " : " << s.second << "권" << endl;
        }
    }
};

int main()
{
    BookManager manager;
    BorrowManager borrower;

    // 도서관 관리 프로그램의 기본 메뉴를 반복적으로 출력하여 사용자 입력을 처리합니다.
    // 프로그램 종료를 선택하기 전까지 계속 동작합니다.
    while (true) 
    {
        cout << "\n도서관 관리 프로그램" << endl;
        cout << "1. 책 추가" << endl; // 책 정보를 입력받아 책 목록에 추가
        cout << "2. 모든 책 출력" << endl; // 현재 책 목록에 있는 모든 책 출력
        cout << "3. 책 제목으로 검색" << endl; 
        cout << "4. 작가의 책 검색" << endl; 
        cout << "5. 도서 대여" << endl; 
        cout << "6. 도서 반납" << endl; 
        cout << "7. 재고 확인" << endl; 
        cout << "8. 종료" << endl; // 프로그램 종료
        cout << "선택: ";

        int choice; // 사용자의 메뉴 선택을 저장
        // 아예 잘못된 입력(숫자가 아닌 경우)시 무한루프 발생현상 방지
        if (!(cin >> choice)) 
        {
            // 추출 실패 시
            cin.clear();                                      // 에러 플래그 해제
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 버퍼에 남은 입력 전체 버리기
            cout << "잘못된 입력입니다. 숫자를 입력하세요.\n";
            continue;                                         // 다음 반복으로
        }

        if (choice == 1) 
        {
            // 1번 선택: 책 추가
            // 사용자로부터 책 제목과 저자명을 입력받아 BookManager에 추가합니다.
            string title, author;
            cout << "책 제목: ";
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            getline(cin, title); // 제목 입력 (공백 포함)
            cout << "책 저자: ";
            getline(cin, author); // 저자명 입력 (공백 포함)
            manager.addBook(title, author); // 입력받은 책 정보를 추가

            // 새로 추가된 책에 대해 재고 초기화
            if (Book* b = manager.getBookByTitle(title)) 
            {
                borrower.initializeStock(*b);
            }
        }
        else if (choice == 2)
        {
            // 2번 선택: 모든 책 출력
            // 현재 BookManager에 저장된 책 목록을 출력합니다.
            manager.displayAllBooks();
        }
        else if (choice == 3) 
        {
            string title;
            cout << "책 제목: ";
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            getline(cin, title); // 제목 입력 (공백 포함)
            manager.searchByTitle(title);
        }
        else if (choice == 4)
        {
            string author;
            cout << "책 저자: ";
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            getline(cin, author); // 저자 입력 (공백 포함)
            manager.searchByAuthor(author);
        }
        else if (choice == 5)
        {
            string title;
            cout << "대여할 책 제목: ";
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            getline(cin, title);
            borrower.borrowBook(title);
        }
        else if (choice == 6)
        {
            string title;
            cout << "반납할 책 제목: ";
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            getline(cin, title);
            borrower.returnBook(title);
        }
        else if (choice == 7)
        {
            borrower.displayStock();
        }
        else if (choice == 8) 
        {
            // 프로그램을 종료하고 사용자에게 메시지를 출력합니다.
            cout << "프로그램을 종료합니다." << endl;
            break; // while 루프 종료
        }
        else
        {
            // 잘못된 입력 처리
            // 메뉴에 없는 번호를 입력했을 경우 경고 메시지를 출력합니다.
            cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }

    return 0; // 프로그램 정상 종료
};

