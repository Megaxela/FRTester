//
// Created by megaxela on 01.12.16.
//

#ifndef FRTESTER_PYTHONTRIGGER_H
#define FRTESTER_PYTHONTRIGGER_H


#include <include/Testing/AbstractTriggerTest.h>
#include <include/Testing/TestEnvironment.h>
#include <Python.h>

/**
 * @brief Триггер, обрабатывающий любой
 * python триггер. Фактически - обертка
 * вокрук python теста.
 */
class PythonTrigger : public AbstractTriggerTest
{
public:
    PythonTrigger(TestEnvironment *environment,
                  const std::string &name,
                  const std::string &description,
                  bool critical,
                  const std::vector<std::string> &tags);

    ~PythonTrigger();

    static TriggerTestPtr loadTrigger(
            TestEnvironment *environment,
            const std::string &moduleName,
            const std::string &modulePath
    );

    void onPreExecute(const std::string &realTag) override;

    void onPostExecute() override;

    bool succeed() override;

    bool containsTag(const std::string &tag) override;

private:
    PyObject* m_pEnvironment;

    PyObject* m_pTriggerName;

    PyObject* m_pModule;

    PyObject* m_object;

    std::vector<std::string> m_tags;
};


#endif //FRTESTER_PYTHONTRIGGER_H
