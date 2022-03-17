/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_GETVERSIONJOB_H
#define SCHAUER_GETVERSIONJOB_H

#include "schauer_global.h"
#include "job.h"

namespace Schauer {

class GetVersionJobPrivate;

/*!
 * \ingroup api-jobs
 * \brief Queries version information from the Docker daemon.
 *
 * Use this class to get version information from the running Docker daemon.
 * There is also AbstractVersionModel to provide this information.
 *
 * Have a look at the description of the Job class to learn how to use Job
 * classes.
 *
 * \par API route
 * /version
 *
 * \par API method
 * GET
 *
 * \dockerAPI{SystemVersion}
 *
 * \headerfile "" <Schauer/GetVersionJob>
 */
class SCHAUER_LIBRARY GetVersionJob : public Job
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %GetVersionJob class with the given \a parent.
     */
    explicit GetVersionJob(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetVersionJob class.
     */
    ~GetVersionJob() override;

    /*!
     * \brief Starts querying the versions from the Docker daemon asynchronously.
     *
     * When the job is finished, result() is emitted.
     * To query the versions in a synchronous way, use exec().
     */
    void start() override;

private:
    Q_DECLARE_PRIVATE_D(s_ptr, GetVersionJob)
    Q_DISABLE_COPY(GetVersionJob)
};

}

#endif // SCHAUER_GETVERSIONJOB_H
