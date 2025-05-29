#ifndef FILE_SYS_ENTRY_HPP
#define FILE_SYS_ENTRY_HPP

#include <QString>
#include <QVariant> // Để trả về QVariant từ hàm data()
#include <sys/stat.h> // For mode_t (quan trọng cho statBuf.st_mode)

// Forward declarations
class Directory;

class File_Sys_Entry {
public:
    enum EntryType {
        FileEntry,
        DirectoryEntry,
        UnknownEntry
    };

    File_Sys_Entry(const QString& name, const QString& full_path, mode_t mode, File_Sys_Entry* parent = nullptr);
    virtual ~File_Sys_Entry() = default;

    virtual void print_Infor(int depth) const = 0;
    virtual bool is_File() const { return false; }
    virtual bool is_Directory() const { return false; }


    QString name() const { return m_name; }
    QString fullPath() const { return m_fullPath; }
    mode_t mode() const { return m_mode; }
    EntryType type() const { return m_type; }


    virtual QVariant data(int role) const;

    void setParentItem(File_Sys_Entry* parent) { m_parentItem = parent; }
    File_Sys_Entry* getParentItem() const { return m_parentItem; }

    virtual int row() const = 0;

protected:
    QString m_name;
    QString m_fullPath;
    mode_t m_mode;
    EntryType m_type;
    File_Sys_Entry* m_parentItem;
};

// Implementations for File_Sys_Entry's methods

inline File_Sys_Entry::File_Sys_Entry(const QString& name, const QString& full_path, mode_t mode, File_Sys_Entry* parent)
    : m_name(name), m_fullPath(full_path), m_mode(mode), m_type(UnknownEntry), m_parentItem(parent) {}

inline QVariant File_Sys_Entry::data(int role) const {
    switch (role) {
    case Qt::DisplayRole:
    case Qt::UserRole + 1:
        return m_name;
    case Qt::UserRole + 2:
        return m_fullPath;
    case Qt::UserRole + 3:
        return (m_type == FileEntry) ? QString("File") : (m_type == DirectoryEntry ? QString("Directory") : QString("Unknown"));

    default:
        return QVariant();
    }
}

#endif // FILE_SYS_ENTRY_HPP
