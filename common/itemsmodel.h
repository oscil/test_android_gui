#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QDebug>

class Item{
public:
    inline Item():
        m_name("Undefined"),
        m_originalName("Undefined"),
        m_itemID(0),
        m_description("Undefined description"),
        m_year(0),
        m_isSkipped(false),
        m_userItemRate(-10000),
        m_isIsUserRate(false),
        m_poster("")
    {}
    inline QString name() {return m_name;}
    inline void setName(QString name) {m_name = name;}
    inline QString originalName() {return m_originalName;}
    inline void setOriginalName(QString name) {m_originalName = name;}
    inline qint32 itemID() {return m_itemID;}
    inline void setItemID(qint32 id) {m_itemID = id;}
    inline QString description() {return m_description;}
    inline void setDescription(QString descr) {m_description = descr;}
    inline quint32 year() {return m_year;}
    inline void setYear(quint32 year) {m_year = year;}
    inline bool isSkipped() {return m_isSkipped;}
    inline void setSkipped(bool skip) {m_isSkipped = skip; m_isIsUserRate = true;}
    inline double userItemRate() {return m_userItemRate;}
    inline void setUserItemRate(double rate) {m_userItemRate = rate; m_isIsUserRate = true;m_isSkipped = false;}
    inline bool isUserRate() {return m_isIsUserRate;}
    inline QString poster() {return m_poster;}
    inline void setPoster(QString poster) {m_poster = poster;}

private:
    QString m_name;
    QString m_originalName;
    qint32 m_itemID;
    QString m_description;
    qint32 m_year;
    bool m_isSkipped;
    double m_userItemRate;
    bool m_isIsUserRate;
    QString m_poster;
};

class ItemsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        UserRateRole = Qt::UserRole + 1,
        UserMadeRateRole,
        UserSkipRole,
        DescriptionRole,
        ItemNameRole,
        ItemOriginalNameRole,
        ItemIDRole,
        ItemYearRole,
        ItemPosterRole
    };

    ItemsModel(QObject* parent = 0);
    ~ItemsModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QHash<int, QByteArray> roleNames() const;
    inline void addItem(Item* item) {
        qDebug() << "ADD ITEM TO MODEL!!!";
        beginResetModel();
        m_itemsList.append(item);
        endResetModel();
    }
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

//    Q_INVOKABLE void add();

private:
    QList<Item*> m_itemsList;
};

#endif // ITEMSMODEL_H
