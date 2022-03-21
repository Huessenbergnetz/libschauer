/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_JOB_H
#define SCHAUER_JOB_H

#include "schauer_exports.h"
#if defined(SCHAUER_WITH_KDE)
#include <KF5/KCoreAddons/KJob>
#else
#include "sjob.h"
#endif
#include "abstractconfiguration.h"
#include <QJsonDocument>
#include <memory>

namespace Schauer {

/*!
 * \defgroup api-jobs API Jobs
 * \brief Classes used to communicate with the Docker REST API.
 */

#if defined(SCHAUER_WITH_KDE)
using SJob = KJob;
#endif

class JobPrivate;
class AbstractNamFactory;

/*!
 * \brief Error codes
 */
enum {
    MissingConfig = SJob::UserDefinedError + 1, /**< AbstractConfiguration is missin on this API class. */
    MissingHost,                /**< The remote host is missing. */
    MissingUser,                /**< The username is missing. */
    MissingPassword,            /**< The password is missing. */
    AuthNFailed,                /**< Authentication failed, check username and password. */
    AuthZFailed,                /**< Authorization failed. */
    InvalidRequestUrl,          /**< The generated request URL is not valid. */
    RequestTimedOut,            /**< The request timed out. */
    JsonParseError,             /**< Failed to parse JSON response message. */
    SslError,                   /**< Error while performing SSL/TLS encryption. */
    NetworkError,               /**< Network related error. */
    APIError,                   /**< Error returned by the Docker API. */
    EmptyReply,                 /**< The repsone data is empty but that was not expected. */
    EmptyJson,                  /**< The repsone data is empty but that was not expected. */
    WrongOutputType,            /**< The output type is not the expected one. */
    InvalidInput,               /**< Some input data is not valid. */
    UnknownError                /**< An unknown error. */
};

/*!
 * \ingroup api-jobs
 * \brief Base class for all API jobs.
 *
 * This class is used by all classes that perform API requests. It is not meant to be used
 * by itself. It provides basic properties and functions used by all classes that perform
 * API requests.
 *
 * \par Example usages
 *
 * \code{.cpp}
 * auto job = new GetVersionJob(this);
 * connect(job, &Job::result, this, [this](SJob *sjob){
 *     if (sjob->error()) {
 *         handleError(sjob->error(), sjob->errorString());
 *     } else {
 *         Job *job = qobject_cast<Job *>(sjob);
 *         handleResult(job->replyData());
 *     }
 * });
 * job->start();
 * \endcode
 *
 * You can also directly use the JSON result with the succeeded() signal.
 * \code{.cpp}
 * auto job = new GetVersionJob(this);
 * connect(job, &Job::succeeded, this, &MyClass:handleJsonResult);
 * job->start();
 * \endcode
 *
 * Or you can use the synchronous way.
 *
 * \code{.cpp}
 * auto job = new GetVersionJob(this);
 * if (job->exec()) {
 *     handleJsonResult(job->replyData());
 * } else {
 *     handleError(job->error(), job->errorString());
 * }
 * \endcode
 */
class SCHAUER_LIBRARY Job : public SJob
{
    Q_OBJECT
    /*!
     * \brief Pointer to an object providing configuration data.
     *
     * This property holds a pointer to an object of a class derived from AbstractConfiguration
     * that provides configuration information for the API request. It especialls provides the
     * remote host and port. See AbstractConfiguration for more information.
     *
     * You can also set a global default configuration via Schauer::setDefaultConfiguration()
     * that will be used if no local configuration is available.
     *
     * \par Access functions
     * \li AbstractConfiguration *configuration() const
     * \li void setConfiguration(AbstractCofiguration *configuration)
     *
     * \par Notifier signal
     * \li void configurationChanged(AbstractConfiguration *configuration)
     */
    Q_PROPERTY(Schauer::AbstractConfiguration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
public:
    /*!
     * \brief Destroys the %Job object.
     */
    ~Job() override;

    /*!
     * \brief Returns a human readable and translated error string.
     *
     * If SJob::error() returns not \c 0, an error occured and the humand readable
     * description can be returned by this function.
     */
    QString errorString() const override;

    /*!
     * \brief Getter function for the \link Job::configuration configuraion\endlink property.
     * \sa setConfiguration(), configurationChanged()
     */
    AbstractConfiguration *configuration() const;

    /*!
     * \brief Setter function for the \link Job::configuration configuration\endlink property.
     * \sa configuration(), configurationChanged()
     */
    void setConfiguration(AbstractConfiguration *configuration);

    /*!
     * \brief Returns the API reply data after successful request.
     *
     * If the API request has been successful and SJob::error() returns \c 0, this
     * function returns the requested JSON data (if any).
     *
     * \sa succeeded()
     */
    QJsonDocument replyData() const;

Q_SIGNALS:
    /*!
     * \brief Notifier signal for the \link Job::configuration configuration\endlink property.
     * \sa configuration(), setConfiguration()
     */
    void configurationChanged(Schauer::AbstractConfiguration *configuration);

    /*!
     * \brief Emitted when the API request ha been successfully finished.
     *
     * This signal is triggered together with SJob::finished() and SJob::result()
     * if the API request was successful. \a json will contain the data requested
     * from the remote server.
     *
     * \sa replyData()
     */
    void succeeded(const QJsonDocument &json);

    /*!
     * \brief Emitted when the API request has been failed.
     *
     * This signal is triggered together with SJob::finished() and SJob::result()
     * if the API request has been failed. \a errroCode will be the code of the
     * error occured, \a errorString will contain a human-readable error message.
     *
     * \sa error(), errorString()
     */
    void failed(int errorCode, const QString &errorString);

protected:
    /*!
     * \brief Constructs a new %Job object with the given \a parent.
     */
    explicit Job(QObject *parent = nullptr);

    Job(JobPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Performs basic checks, sets up and sends the request.
     *
     * This will be called in the reimplementation of BJob::start() by
     * classes derived from %Job.
     */
    void sendRequest();

    const std::unique_ptr<JobPrivate> s_ptr;

private:
    Q_DECLARE_PRIVATE_D(s_ptr, Job)
    Q_DISABLE_COPY(Job)
};

}

#endif // SCHAUER_JOB_H
