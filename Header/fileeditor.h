#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <QObject>

class fileEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString contents READ contents WRITE setContents NOTIFY contentsChanged FINAL)
public:
    explicit fileEditor(QObject *parent = nullptr);

    QString contents() const;
    void setContents(const QString &newContents);
    Q_INVOKABLE void readFileToRam(const QString &filePath);

signals:

    void contentsChanged();
    void fileContentReady(const QString &content);
    void fileReadError(const QString &errorMessage);

private:
    QString m_contents;
};

#endif // FILEEDITOR_H
