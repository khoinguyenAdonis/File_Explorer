#include "Header/file.hpp"

File::File(const QString& name, const QString& full_path, mode_t mode, File_Sys_Entry* parent)
    : File_Sys_Entry(name, full_path, mode,parent)
{
    m_type = FileEntry;
}

void File::print_Infor(int depth) const
{

}

Directory::Directory(const QString& name, const QString& full_path, mode_t mode,File_Sys_Entry* parent )
    : File_Sys_Entry(name, full_path, mode, parent)
{
    m_type = DirectoryEntry;
}

Directory::~Directory() {

}
int Directory::indexOfChild(const File_Sys_Entry* child) const {
    for (int i = 0; i < m_children.size(); ++i) {
        if (m_children.at(i).get() == child) {
            return i;
        }
    }
    return -1; // Không tìm thấy
}

int Directory::row() const{
    if (m_parentItem) {
        // Cha của một Directory cũng là một Directory
        Directory* parentDir = static_cast<Directory*>(m_parentItem);
        if (parentDir) {
            return parentDir->indexOfChild(this);
        }
    }
    // Nếu là root của cây được quét (gốc của model), hàng của nó là 0.
    return 0;
}

void Directory::print_Infor(int depth) const
{

}

void Directory::add_Child(std::unique_ptr<File_Sys_Entry> child) {
    child->setParentItem(this);
    m_children.push_back(std::move(child));
}

File_Sys_Entry* Directory::child(int row) const {
    if (row < 0 || row >= m_children.size())
        return nullptr;
    return m_children.at(row).get(); // Trả về con trỏ thô
}

int Directory::childCount() const {
    return m_children.size();
}


int File::row() const {
    if (m_parentItem) {
        Directory* parentDir = static_cast<Directory*>(m_parentItem);
        if (parentDir) {
            return parentDir->indexOfChild(this);
        }
    }
    return -1;
}

File::~File()
{

}
std::string Scanner::getBasename(const std::string& dir_path) const{
    size_t lastSlash = dir_path.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
            return dir_path;
        }
    return dir_path.substr(lastSlash + 1);
}

// Implementations for Scanner methods (in .cpp)
std::unique_ptr<Directory> Scanner::scan_Directory(const QString& dir_path, int max_depth) {
    //QFileInfo rootInfo(dir_path);
    struct stat statbuf;
    const char * Path = dir_path.toUtf8().constData();

    if (stat(Path,&statbuf) || !S_ISDIR(statbuf.st_mode)) {
        qWarning() << "Error: Not a valid directory or cannot get info for" << dir_path;
        return nullptr;
    }
    QString fileName = QString::fromStdString(getBasename(Path));
    auto root = std::make_unique<Directory>(fileName,QString::fromUtf8(Path), statbuf.st_mode,nullptr);
    populateDirectory(root.get(), max_depth, 0); // Bắt đầu từ depth 0
    return root;
}

void Scanner::populateDirectory(Directory* currentDir, int max_depth, int current_depth) {
    
    const char * Path =  currentDir->fullPath().toUtf8().constData();

    if (current_depth >= max_depth && max_depth != -1) {
            return;
    }

    DIR *dr;
    struct dirent* entry;
    struct stat statBuf;
    QString fullPath;

    dr = opendir(Path);

    if (dr == NULL){
        qWarning() << "Error: Can't open directory" << Path;
        return;
    }

    while ((entry = readdir(dr)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
        }

        fullPath = currentDir->fullPath() + "/" + entry->d_name;
        QByteArray pathBytes = fullPath.toUtf8();
        Path = pathBytes.constData();
        if (lstat(Path,&statBuf) == -1){
            qWarning() <<" Error: Cannot get info for " << Path;
            continue;
        }

        if(S_ISDIR(statBuf.st_mode)){
            auto subDir = std::make_unique<Directory>(entry->d_name, QString::fromUtf8(Path),statBuf.st_mode,nullptr);
            populateDirectory(subDir.get(), max_depth, current_depth + 1);
            currentDir->add_Child(std::move(subDir));
        }
        else if(S_ISREG(statBuf.st_mode)){
            auto file = std::make_unique<File>(entry->d_name,QString::fromUtf8(Path), statBuf.st_mode,nullptr);
            currentDir->add_Child(std::move(file));
        }
    }
    closedir(dr);
}
