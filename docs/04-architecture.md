# Етап 4. Архітектура та Design Patterns

## 1. Шари застосунку

```
UI (Qt Widgets)  ->  Services (бізнес-логіка)  ->  Domain (сутності, інтерфейси)  <-  Data (SQLite, файли)
```

| Шар          | Вміст                                                                                                        | Знає про              |
| ------------ | ------------------------------------------------------------------------------------------------------------ | --------------------- |
| **Domain**   | `Contact`, `Payment`, `Attachment`, `Money`, `PaymentDirection`, інтерфейси `I...Repository`, `IFileStorage` | Нікого (лише Qt Core) |
| **Services** | `ContactService`, `PaymentService`, `AttachmentService`, `BalanceService`                                    | Domain                |
| **Data**     | `Database`, `Sqlite...Repository`, `LocalFileStorage`                                                        | Domain                |
| **UI**       | Вікна та діалоги                                                                                             | Services, Domain      |

Domain не залежить ні від кого, а Data і Services залежать від нього. Складання всіх частин відбувається в точці входу програми.

## 2. UML-діаграма: ядро та шар даних

```mermaid
classDiagram
    direction TB

    class PaymentDirection {
        <<enumeration>>
        Lent
        Borrowed
    }

    class Money {
        -kopecks qint64
        +fromHryvnias(value) Money
        +toString() QString
    }

    class Contact {
        -id qint64
        -firstName QString
        -lastName QString
        -note QString
        -createdAt QDateTime
        +displayName() QString
        +isValid() bool
    }

    class Payment {
        -id qint64
        -contactId qint64
        -amount Money
        -direction PaymentDirection
        -date QDate
        -comment QString
        +signedAmount() qint64
        +isValid() bool
    }

    class Attachment {
        -id qint64
        -paymentId qint64
        -fileName QString
        -storedPath QString
        -addedAt QDateTime
    }

    class IContactRepository {
        <<interface>>
        +add(contact) bool
        +update(contact) bool
        +remove(id) bool
        +getById(id) optional
        +getAll() QList~Contact~
    }

    class IPaymentRepository {
        <<interface>>
        +add(payment) bool
        +update(payment) bool
        +remove(id) bool
        +getById(id) optional
        +getByContact(contactId) QList~Payment~
        +getAll() QList~Payment~
    }

    class IAttachmentRepository {
        <<interface>>
        +add(attachment) bool
        +remove(id) bool
        +getByPayment(paymentId) QList~Attachment~
    }

    class IFileStorage {
        <<interface>>
        +copyIn(sourcePath) QString
        +removeCopy(storedPath) bool
    }

    class QObject {
        <<Qt>>
    }

    class ContactService {
        +addContact(contact, error) bool
        +updateContact(contact, error) bool
        +removeContact(id) bool
        +getAll() QList~Contact~
        <<signal>> contactsChanged()
    }

    class PaymentService {
        +addPayment(payment, error) bool
        +updatePayment(payment, error) bool
        +removePayment(id) bool
        +getByContact(contactId) QList~Payment~
        <<signal>> paymentsChanged()
    }

    class AttachmentService {
        +attach(paymentId, filePath) bool
        +remove(id) bool
        +removeAllForPayment(paymentId) bool
        +getByPayment(paymentId) QList~Attachment~
    }

    class BalanceService {
        +balanceForContact(payments) Money
        +owedToMe(paymentsByContact) Money
        +iOwe(paymentsByContact) Money
        +total(paymentsByContact) Money
    }

    class Database {
        +open(path) QSqlDatabase
        +createSchema()
    }

    class SqliteContactRepository
    class SqlitePaymentRepository
    class SqliteAttachmentRepository
    class LocalFileStorage

    Contact "1" -- "*" Payment
    Payment "1" -- "*" Attachment
    Payment --> Money
    Payment --> PaymentDirection

    QObject <|-- ContactService
    QObject <|-- PaymentService

    ContactService --> IContactRepository
    ContactService --> PaymentService
    PaymentService --> IPaymentRepository
    PaymentService --> AttachmentService
    AttachmentService --> IAttachmentRepository
    AttachmentService --> IFileStorage
    BalanceService ..> Payment : uses

    SqliteContactRepository ..|> IContactRepository
    SqlitePaymentRepository ..|> IPaymentRepository
    SqliteAttachmentRepository ..|> IAttachmentRepository
    LocalFileStorage ..|> IFileStorage

    SqliteContactRepository --> Database
    SqlitePaymentRepository --> Database
    SqliteAttachmentRepository --> Database
```

## 3. UML-діаграма: шар інтерфейсу

```mermaid
classDiagram
    direction LR

    class MainWindow {
        <<QMainWindow>>
        -refreshSummary()
        -onAddContact()
        -onAddPayment()
        -onContactDoubleClicked()
    }

    class ContactDetailsWindow {
        <<QDialog>>
        -refresh()
        -onAddPayment()
        -onDeletePayment()
    }

    class ContactDialog {
        <<QDialog>>
        +contact() Contact
    }

    class PaymentDialog {
        <<QDialog>>
        +payment() Payment
        +attachedFiles() QStringList
    }

    class ContactService
    class PaymentService
    class AttachmentService
    class BalanceService

    MainWindow --> ContactDetailsWindow
    MainWindow --> ContactDialog
    MainWindow --> PaymentDialog
    MainWindow --> ContactService
    MainWindow --> PaymentService
    MainWindow --> BalanceService
    ContactDetailsWindow --> PaymentDialog
    ContactDetailsWindow --> PaymentService
    ContactDetailsWindow --> BalanceService

    PaymentDialog --> PaymentService
    PaymentDialog --> AttachmentService
    ContactDialog --> ContactService
```

Шар інтерфейсу залежить лише від сервісів і підписується на їхні сигнали. Конкретний вигляд екранів (списки, картки тощо) може змінюватися без впливу на ядро.

## 4. Використані патерни проєктування

| Патерн                             | Де застосовано                                                                                                  | Навіщо                                                                       |
| ---------------------------------- | --------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- |
| **Repository**                     | `I...Repository` та `Sqlite...Repository`                                                                       | Уся робота з SQLite ізольована в одному місці, сервіси не містять SQL        |
| **Dependency Injection**           | Сервіси отримують залежності через конструктор, складання в `main.cpp`                                          | Залежності легко підмінити, наприклад тестовими реалізаціями                 |
| **Observer** (сигнали та слоти Qt) | `ContactService` та `PaymentService` випромінюють `contactsChanged()` / `paymentsChanged()`, вікна підписуються | Інтерфейс оновлюється автоматично після змін даних, сервіси не знають про UI |
| **Service Layer**                  | `ContactService`, `PaymentService`, `AttachmentService`, `BalanceService`                                       | Уся бізнес-логіка зібрана в одному шарі, інтерфейс лише викликає сервіси     |
| **Value Object**                   | `Money`                                                                                                         | Гроші інкапсульовані: сума в копійках без похибок, єдине форматування        |

## 5. Реалізація принципів ООП

- **Інкапсуляція:** поля сутностей приватні, доступ через методи, валідація всередині класів (`isValid()`).
- **Абстракція:** сервіси працюють з інтерфейсами репозиторіїв і файлового сховища.
- **Поліморфізм:** `SqlitePaymentRepository` та будь-яка інша реалізація `IPaymentRepository` взаємозамінні для `PaymentService`.
- **Композиція замість успадкування:** сервіси складаються з залежностей, а успадкування застосоване лише там, де цього вимагає Qt.
