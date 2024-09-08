#include "crow.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>


std::string hospitalNumber;
namespace fs = boost::filesystem;
int cur_img = 0;

std::string join(const std::vector<std::string>& elements, const std::string& delimiter) {
    std::ostringstream os;
    auto it = elements.begin();
    if (it != elements.end()) {
        os << *it++;
        while (it != elements.end()) {
            os << delimiter << *it++;
        }
    }
    return os.str();
}

using namespace std;

// 文件保存路径
const std::string UPLOAD_DIR = "./uploads/";

// 辅助函数：从 multipart/form-data 中提取文件名和内容
bool parse_multipart(const std::string& boundary, const std::string& body, std::string& file_name, std::string& file_content) {
    std::string dash_dash = "--";
    std::string crlf = "\r\n";
    size_t pos = 0;
    size_t start, end;

    while ((start = body.find(dash_dash + boundary, pos)) != std::string::npos) {
        start += dash_dash.length() + boundary.length();
        if ((end = body.find(crlf + crlf, start)) == std::string::npos) {
            return false;
        }

        std::string header = body.substr(start, end - start);
        pos = end + 2 * crlf.length();

        if ((start = header.find("filename=\"")) != std::string::npos) {
            start += 10;
            if ((end = header.find("\"", start)) == std::string::npos) {
                return false;
            }
            file_name = header.substr(start, end - start);
            if ((end = body.find(dash_dash + boundary, pos)) == std::string::npos) {
                return false;
            }
            file_content = body.substr(pos, end - pos - 2);
            return true;
        }
    }
    return false;
}

int main_miniprogram() {
    crow::SimpleApp app;

    // 数据库连接参数
    const std::string db_host = "localhost";
    const std::string db_user = "root";
    const std::string db_pass = "123456"; // 更换为你的数据库密码
    const std::string db_name = "testc";  // 更换为你的数据库名称

    // 数据库连接
    sql::mysql::MySQL_Driver *driver;
    std::shared_ptr<sql::Connection> con;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con.reset(driver->connect("tcp://" + db_host, db_user, db_pass));
        con->setSchema(db_name);
    } catch (sql::SQLException &e) {
        std::cerr << "Database connection failed: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Connected to database." << std::endl;

    // 获取数据库中的表列表
    CROW_ROUTE(app, "/tables")
    ([&con]() {
        crow::json::wvalue tables;
        try {
            std::shared_ptr<sql::Statement> stmt(con->createStatement());
            std::shared_ptr<sql::ResultSet> res(stmt->executeQuery("SHOW TABLES"));

            int i = 0;
            while (res->next()) {
                tables[i++] = res->getString(1);
            }
        } catch (sql::SQLException &e) {
            std::cerr << "Error fetching tables: " << e.what() << std::endl;
            return crow::response(500, "Error fetching tables");
        }
        return crow::response(tables);
    });

    // 获取指定表的数据
    CROW_ROUTE(app, "/data/<string>")
    ([&con](const std::string& tableName) {
        crow::json::wvalue data;
        try {
            std::shared_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM " + tableName));
            std::shared_ptr<sql::ResultSet> res(pstmt->executeQuery());

            int row = 0;
            while (res->next()) {
                crow::json::wvalue row_data;
                sql::ResultSetMetaData* meta = res->getMetaData();
                int column_count = meta->getColumnCount();
                for (int col = 1; col <= column_count; ++col) {
                    row_data[meta->getColumnName(col)] = res->getString(col);
                }
                data[row++] = std::move(row_data);
            }
        } catch (sql::SQLException &e) {
            std::cerr << "Error fetching data from table " << tableName << ": " << e.what() << std::endl;
            return crow::response(500, "Error fetching data from table " + tableName);
        }
        return crow::response(data);
    });

    // 上传文件处理
    // CROW_ROUTE(app, "/upload").methods("POST"_method)
    // ([](const crow::request& req) {
    //     std::string content_type = req.get_header_value("Content-Type");
    //     std::string boundary = content_type.substr(content_type.find("boundary=") + 9);

    //     std::string file_name;
    //     std::string file_content;

    //     cout<<"content_type: "<<content_type<<endl;
    //     cout<<"boundary: "<<boundary<<endl;
    //     std::cout << req.body << std::endl;

    //     if (!parse_multipart(boundary, req.body, file_name, file_content)) {
    //         return crow::response(400, "Invalid multipart/form-data");
    //     }

    //     // 保存文件
    //     std::ofstream out(UPLOAD_DIR + file_name, std::ios::binary);
    //     if (!out) {
    //         return crow::response(500, "Failed to save file");
    //     }
    //     out.write(file_content.data(), file_content.size());
    //     out.close();

    //     crow::json::wvalue response;
    //     response["status"] = "success";
    //     response["file_name"] = file_name;
    //     return crow::response(200, response);
    // });

    CROW_ROUTE(app, "/upload").methods("POST"_method)
    ([](const crow::request& req) {
        auto content_type = req.get_header_value("Content-Type");
        std::string boundary = content_type.substr(content_type.find("boundary=") + 9);

        crow::multipart::message msg(req);
        
        if (msg.parts.empty()) {
            return crow::response(400, "Invalid multipart/form-data");
        }
        
        // cout<<"cur_img: "<<cur_img<<endl;
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << hospitalNumber;
        std::string formattedHospitalNumber = oss.str();
        std::cout << "Formatted hospital number: " << formattedHospitalNumber << std::endl;
        string upload_dir = "./dataset/"+formattedHospitalNumber+"/";
        
        if(cur_img == 0){
            for (fs::directory_iterator it(upload_dir); it != fs::directory_iterator(); ++it) {
                fs::remove(it->path());
            }
            cur_img++;
        }else if(cur_img == 4){
            cur_img = 0;
        }else{
            cur_img++;
        }


        for (auto& part : msg.parts) {
            auto filename_header = part.get_header_object("Content-Disposition");
            if (filename_header.params.find("filename") != filename_header.params.end()) {
                std::string filename = filename_header.params["filename"];
                std::ofstream out(upload_dir + filename, std::ios::binary);
                out << part.body;
                out.close();
            }
        }

        return crow::response(200, "File(s) uploaded successfully");
    });

    // 处理表单数据上传
    CROW_ROUTE(app, "/upload_data").methods("POST"_method)
    ([&con](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) {
            return crow::response(400, "Invalid JSON");
        }
        cur_img = 0;
        hospitalNumber = data["hospitalNumber"].s();
        std::string name = data["name"].s();
        std::string age = data["age"].s();
        std::string gender = data["gender"].s();
        std::string phone = data["phone"].s();
        cout<<"hospitalNumber: "<<hospitalNumber<<endl;
        cout<<"name: "<<name<<endl;
        cout<<"age: "<<age<<endl;
        cout<<"gender: "<<gender<<endl;
        cout<<"phone: "<<phone<<endl;

        try {
            // 检查是否存在相同的住院号
            std::shared_ptr<sql::PreparedStatement> check_stmt(con->prepareStatement(
                "SELECT COUNT(*) FROM information WHERE faceid = ?"
            ));
            check_stmt->setString(1, hospitalNumber);
            std::shared_ptr<sql::ResultSet> res(check_stmt->executeQuery());
            res->next();
            int count = res->getInt(1);

            if (count > 0) {
                std::string update_query = "UPDATE information SET ";
                std::vector<std::string> updates;
                std::vector<std::string> values;

                if (!name.empty()) {
                    updates.push_back("姓名 = ?");
                    values.push_back(name);
                }
                if (!age.empty()) {
                    updates.push_back("年龄 = ?");
                    values.push_back(age);
                }
                if (!gender.empty()) {
                    updates.push_back("性别 = ?");
                    values.push_back(gender);
                }

                // 如果没有任何字段需要更新
                if (updates.empty()) {
                    return crow::response(400, "No fields to update");
                }

                update_query += join(updates, ", ") + " WHERE faceid = ?";
                cout<<"update_query: "<<update_query<<endl;
                values.push_back(hospitalNumber);

                std::shared_ptr<sql::PreparedStatement> update_stmt(con->prepareStatement(update_query));

                for (size_t i = 0; i < values.size(); ++i) {
                    update_stmt->setString(i + 1, values[i]);
                }

                update_stmt->executeUpdate();
            } else {
                // 如果不存在，插入新记录
                std::shared_ptr<sql::PreparedStatement> insert_stmt(con->prepareStatement(
                    "INSERT INTO information (faceid, 姓名, 年龄, 性别) VALUES (?, ?, ?, ?)"
                ));
                insert_stmt->setString(1, hospitalNumber);
                insert_stmt->setString(2, name);
                insert_stmt->setString(3, age);
                insert_stmt->setString(4, gender);

                insert_stmt->executeUpdate();
            }

        } catch (sql::SQLException &e) {
            std::cerr << "Error inserting/updating data: " << e.what() << std::endl;
            return crow::response(500, "Error inserting/updating data");
        }

        crow::json::wvalue response;
        response["status"] = "success";
        return crow::response(200, response);
    });

    app.port(3000).multithreaded().run();

    return 0;
}
