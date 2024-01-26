#include <pqxx/pqxx>
#include <iostream>

// running Postgres via docker local
const std::string host = "localhost";
const std::string port = "5432";
const std::string dbname = "postgres";
const std::string user = "postgres";
const std::string password = "mys";

const std::string connString = "dbname=" + dbname +
                               " user=" + user +
                               " password=" + password +
                               " host=" + host +
                               " port=" + port;

void createTable()
{
    pqxx::connection con(connString);
    pqxx::work txn(con);
    const std::string createTable = R"(
            CREATE TABLE IF NOT EXISTS users(
                id SERIAL PRIMARY KEY,
                name VARCHAR(100),
                mail VARCHAR(100),
                sha VARCHAR(100)
            );
            CREATE TABLE IF NOT EXISTS temp (
                id SERIAL PRIMARY KEY, 
                user_name Text NOT NULL, 
                pw_sha VARCHAR(100)
            );
        )";

    txn.exec(createTable);
    txn.commit();
};

void insertUser(std::string name, std::string sha, std::string mail)
{
    pqxx::connection conn(connString);
    pqxx::work txn(conn);

    std::string sqlInsert = "INSERT INTO users (name, mail, sha)VALUES ('" + txn.esc(name) +
                            "','" + txn.esc(mail) + "','" + txn.esc(sha) + "');";
    txn.exec(sqlInsert);
    txn.commit();

    std::cout << "Added User: " << name << std::endl;
};

void dropTable(const std::string nameOfTable)
{
    try
    {
        pqxx::connection conn(connString);
        pqxx::work txn(conn);
        std::string sql = "DROP TABLE IF EXISTS " + txn.quote_name(nameOfTable);
        txn.exec(sql);
        txn.commit();
        std::cout << "Table dropped successfully." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        // Handle the exception as needed
    }
}

bool dbCheckConnection()
{
    pqxx::connection conn(connString);

    if (conn.is_open())
    {
        std::cout << "Connected to database successfully." << std::endl;
        createTable(); // creat all needed Tables if not existing
    }
    else
    {
        std::cerr << "Failed to connect to database." << std::endl;
        return false;
    }
    return true;
};

void testDB(){
    // remove hard code later
    // dropTable(std::string("test2"));
    // insertUser(std::string("Christian"), std::string("christian@me.com"), std::string("sha"));
};

int main()
{
    bool dbOnline = dbCheckConnection();
    if (dbOnline == false)
    {
        std::cout << "No DB connection" << std::endl;
        return 1; // maybe re-try >> next step add logger functionality
    }

    try
    {
        // add here
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
