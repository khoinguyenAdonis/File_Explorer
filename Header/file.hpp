#include "abstractClass.hpp"
#include <memory>     // For std::unique_ptr
#include <dirent.h>   // For opendir, readdir, closedir
#include <sys/stat.h> // For stat, lstat, S_ISDIR, S_ISREG, S_ISLNK etc.
#include <cstring>    // For strcmp
#include <QString>
#include <vector>

class File : public File_Sys_Entry {
public:
    File(const QString& name, const QString& full_path, mode_t mode, File_Sys_Entry* parent = nullptr);
    bool is_File() const override { return true; }
    void print_Infor(int depth) const override;
    int row() const override;
    ~File();
};



class Directory : public File_Sys_Entry {
public:
    Directory(const QString& name, const QString& full_path, mode_t mode, File_Sys_Entry* parent = nullptr);
    ~Directory();

    void add_Child(std::unique_ptr<File_Sys_Entry> child);
    File_Sys_Entry* child(int row) const; // Trả về con trỏ thô để Model truy cập
    int childCount() const;
    inline int indexOfChild(const File_Sys_Entry* child) const; // Cần thêm khai báo này
    inline int row() const override; // Khai báo này đã có, cần triển khai

    void print_Infor(int depth) const override;
    bool is_Directory() const override { return true; }

private:
    std::vector<std::unique_ptr<File_Sys_Entry>> m_children;
};



class Scanner {
private:
    // Hàm helper để tìm cha của một đường dẫn
    std::string getBasename(const std::string& dir_path) const;

    // Đệ quy populate thư mục
    void populateDirectory(Directory* currentDir, int max_depth, int current_depth);

public:
    // Hàm chính để bắt đầu quét
    std::unique_ptr<Directory> scan_Directory(const QString& dir_path, int max_depth);
};


