#pragma once

#include <QAbstractListModel>

class ModelItem {
    Q_GADGET

    Q_PROPERTY(int value READ value)

public:
    ModelItem();
    ~ModelItem();

    int value() const;
    void setValue(int value);

private:
    int _value;
};

Q_DECLARE_METATYPE(ModelItem);

/**
 * @brief Model for qml interface.
 *
 */
class Model: public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    /**
     * @brief Construct a new Model.
     *
     * @param parent
     */
    Model(QObject* parent = nullptr);

    /**
     * @brief Roles
     *
     */
    enum Roles {
        ModelData,
    };

    /**
     * @brief Return data.
     *
     * @param index
     * @param role
     * @return QVariant
     */
    QVariant data(const QModelIndex& index, int role) const override;

    /**
     * @brief Get role names.
     *
     * @return QHash<int, QByteArray>
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Return model size
     *
     * @param parent
     * @return int
     */
    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return m_size;
    };

signals:
    void countChanged();

    /**
     * @brief Append item to model.
     *
     * @param item
     */
    void append(const ModelItem& item);

private:
    Q_DISABLE_COPY(Model)

    /**
     * @brief Do append inside model thread.
     *
     * @param item
     */
    void doAppend(const ModelItem& item);

    int m_categories = 0;
    QHash<int, QByteArray> m_roleNames {
        {Model::Roles::ModelData, "modelData"},
    };
    int m_size = 0;
    QHash<int, QVector<QVariant>> m_vectors;
};

Q_DECLARE_METATYPE(Model*)
