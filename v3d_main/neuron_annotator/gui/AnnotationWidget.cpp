#include "AnnotationWidget.h"
#include "ui_AnnotationWidget.h"
#include "trees/OntologyTreeModel.h"
#include "trees/AnnotatedBranchTreeModel.h"
#include "trees/EntityTreeItem.h"
#include "NaMainWindow.h"
#include "../entity_model/Entity.h"
#include "../entity_model/Ontology.h"
#include "../entity_model/OntologyAnnotation.h"
#include "../../webservice/impl/ConsoleObserverServiceImpl.h"
#include "../utility/ConsoleObserver.h"
#include "../utility/DataThread.h"
#include <QModelIndex>
#include <QtGui>

// Compiler bug workaround borrowed from 3drenderer/qtr_widget.h
#ifdef Q_WS_X11
#define QEVENT_KEY_PRESS 6 //for crazy RedHat error: expected unqualified-id before numeric constant
#else
#define QEVENT_KEY_PRESS QEvent::KeyPress
#endif

#define LABEL_NONE "None"
#define LABEL_CONSOLE_DICONNECTED "Console not connected"
#define LABEL_CONSOLE_CONNECTED "Console connected"
#define BUTTON_CONNECT "Connect"
#define BUTTON_CONNECTING "Connecting..."
#define BUTTON_DISCONNECT "Disconnect"
#define BUTTON_DISCONNECTING "Disconnecting..."

AnnotationWidget::AnnotationWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AnnotationWidget),
    mutex(QMutex::Recursive),
    ontology(0),
    ontologyTreeModel(0),
    annotatedBranch(0),
    annotatedBranchTreeModel(0),
    consoleObserverService(0),
    consoleObserver(0),
    naMainWindow(0),
    firstLoad(true)
{
    ui->setupUi(this);
    connect(ui->ontologyTreeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ontologyTreeDoubleClicked(QModelIndex)));
    connect(ui->annotatedBranchTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(annotatedBranchTreeClicked(QModelIndex)));
    connect(ui->annotatedBranchTreeView, SIGNAL(removeAnnotation(const Entity*)), this, SLOT(removeAnnotation(const Entity*)));
    showDisconnected();
}

AnnotationWidget::~AnnotationWidget()
{
    closeOntology();
    closeAnnotatedBranch();
    if (ui != 0) delete ui;
    if (consoleObserver != 0) delete consoleObserver;
    consoleObserverService->stopServer(); // will delete itself later
    createAnnotationThread->disregard(); // will delete itself later
    removeAnnotationThread->disregard(); // will delete itself later
}

void AnnotationWidget::setMainWindow(NaMainWindow *mainWindow)
{
    naMainWindow = mainWindow;
}

void AnnotationWidget::closeOntology()
{
    QMutexLocker locker(&mutex);

    if (this->ontology != 0)
    {
        delete this->ontology;
        this->ontology = 0;
    }

    if (ui->ontologyTreeView->model() != 0)
    {
        delete ui->ontologyTreeView->model();
        ui->ontologyTreeView->setModel(0);
    }
}

void AnnotationWidget::openOntology(Ontology *ontology)
{
    QMutexLocker locker(&mutex);

    closeOntology();
    this->ontology = ontology;

    if (ontology != NULL && ontology->root() != NULL && ontology->root()->name != NULL)
    {
        ui->ontologyTreeTitle->setText(*ontology->root()->name);

        ontologyTreeModel = new OntologyTreeModel(ontology);
        ui->ontologyTreeView->setModel(ontologyTreeModel);
        ui->ontologyTreeView->expandAll();

        if (firstLoad)
        {
            ui->ontologyTreeView->header()->swapSections(1,0);
            firstLoad = false;
        }

        ui->ontologyTreeView->resizeColumnToContents(0);
        ui->ontologyTreeView->resizeColumnToContents(1);
    }
    else
    {
        ui->ontologyTreeTitle->setText("Error loading ontology");
    }
}

void AnnotationWidget::closeAnnotatedBranch()
{
    QMutexLocker locker(&mutex);

    if (this->annotatedBranch != 0)
    {
        delete this->annotatedBranch;
        this->annotatedBranch = 0;
    }

    if (ui->annotatedBranchTreeView->model() != 0)
    {
        delete ui->annotatedBranchTreeView->model();
        ui->annotatedBranchTreeView->setModel(0);
    }

    this->selectedEntity = 0;
}

void AnnotationWidget::openAnnotatedBranch(AnnotatedBranch *annotatedBranch, bool openStack)
{
    QMutexLocker locker(&mutex);

    if (annotatedBranch == 0 || annotatedBranch->entity() == NULL)
    {
        ui->annotatedBranchTreeTitle->setText("Error loading annotations");
        closeAnnotatedBranch();
        return;
    }

    bool reload = (this->annotatedBranch != 0 && *annotatedBranch->entity()->id == *this->annotatedBranch->entity()->id);
    qint64 selectedEntityId = selectedEntity==0?-1:*selectedEntity->id;

    // Don't delete things if we're just reopening the same branch
    if (annotatedBranch != this->annotatedBranch)
    {
        closeAnnotatedBranch();
        this->annotatedBranch = annotatedBranch;
    }

    annotatedBranchTreeModel = new AnnotatedBranchTreeModel(annotatedBranch);
    ui->annotatedBranchTreeView->setModel(annotatedBranchTreeModel);
    ui->annotatedBranchTreeView->expandAll();
    ui->annotatedBranchTreeView->resizeColumnToContents(0);
    if (openStack) naMainWindow->openMulticolorImageStack(annotatedBranch->getFilePath());

    // Reselect the entity that was previously selected
    if (reload && selectedEntityId >= 0)
        ui->annotatedBranchTreeView->selectEntity(selectedEntityId);
}

void AnnotationWidget::updateAnnotations(long entityId, AnnotationList *annotations)
{
    QMutexLocker locker(&mutex);

    if (this->annotatedBranch == 0) return;

    // The next step could cause our currently selected annotation to be deleted, so lets make sure to deselect it.
    QListIterator<Entity *> i(*annotatedBranch->getAnnotations(entityId));
    while (i.hasNext())
    {
        Entity *annot = i.next();
        if (selectedEntity == annot) selectedEntity = 0;
    }

    annotatedBranch->updateAnnotations(entityId, annotations);

    // TODO: this can be made more efficient in the future, for now let's just recreate it from scratch
    openAnnotatedBranch(annotatedBranch, false);
}

void AnnotationWidget::communicationError(const QString & errorMessage)
{
    QString msg = QString("Error communicating with the Console: %1").arg(errorMessage);
    showErrorDialog(msg);
}

void AnnotationWidget::showErrorDialog(const QString & text)
{
    QMessageBox msgBox(QMessageBox::Critical, "Error", text, QMessageBox::Ok, this);
    msgBox.exec();
}

void AnnotationWidget::showDisconnected()
{
    QMutexLocker locker(&mutex);

    // Clear the ontology
    ui->ontologyTreeView->setModel(NULL);
    if (this->ontology != 0) delete this->ontology;
    this->ontology = 0;
    ui->ontologyTreeTitle->setText(LABEL_NONE);

    // Clear the annotations
    ui->annotatedBranchTreeView->setModel(NULL);
    if (this->annotatedBranch != 0) delete this->annotatedBranch;
    this->annotatedBranch = 0;

    // Reset the console link
    ui->consoleLinkLabel->setText(LABEL_CONSOLE_DICONNECTED);
    ui->consoleLinkButton->setText(BUTTON_CONNECT);
    ui->consoleLinkButton->setEnabled(true);
    ui->consoleLinkButton->disconnect();
    connect(ui->consoleLinkButton, SIGNAL(clicked()), this, SLOT(consoleConnect()));
}

void AnnotationWidget::consoleConnect() {

    QMutexLocker locker(&mutex);

    ui->consoleLinkButton->setText(BUTTON_CONNECTING);
    ui->consoleLinkButton->setEnabled(false);

    consoleObserver = new ConsoleObserver(naMainWindow);
    connect(consoleObserver, SIGNAL(openAnnotatedBranch(AnnotatedBranch*)), this, SLOT(openAnnotatedBranch(AnnotatedBranch*)));
    connect(consoleObserver, SIGNAL(openOntology(Ontology*)), this, SLOT(openOntology(Ontology*)));
    connect(consoleObserver, SIGNAL(updateAnnotations(long,AnnotationList*)), this, SLOT(updateAnnotations(long,AnnotationList*)));
    connect(consoleObserver, SIGNAL(communicationError(const QString&)), this, SLOT(communicationError(const QString&)));

    consoleObserverService = new obs::ConsoleObserverServiceImpl();

    if (consoleObserverService->errorMessage()!=0)
    {
        QString msg = QString("Could not connect to the Console: %1").arg(*consoleObserverService->errorMessage());
        showErrorDialog(msg);
        showDisconnected();
        return;
    }

    qDebug() << "Received console approval to run on port:"<<consoleObserverService->port();

    connect(consoleObserverService, SIGNAL(ontologySelected(long)), consoleObserver, SLOT(ontologySelected(long)));
    connect(consoleObserverService, SIGNAL(ontologyChanged(long)), consoleObserver, SLOT(ontologyChanged(long)));
    connect(consoleObserverService, SIGNAL(entitySelected(long)), consoleObserver, SLOT(entitySelected(long)));
    connect(consoleObserverService, SIGNAL(entityViewRequested(long)), consoleObserver, SLOT(entityViewRequested(long)));
    connect(consoleObserverService, SIGNAL(annotationsChanged(long)), consoleObserver, SLOT(annotationsChanged(long)));
    consoleObserverService->startServer();

    if (consoleObserverService->error!=0)
    {
        consoleObserverService->stopServer(); // will delete itself later
        qDebug() << "Could not start Console observer, error code:" << consoleObserverService->error;
        QString msg = QString("Could not bind to port %1").arg(consoleObserverService->port());
        showErrorDialog(msg);
        showDisconnected();
        return;
    }

    consoleObserverService->registerWithConsole();

    if (consoleObserverService->errorMessage()!=0)
    {
        consoleObserverService->stopServer(); // will delete itself later
        QString msg = QString("Could not register with the Console: %1").arg(*consoleObserverService->errorMessage());
        showErrorDialog(msg);
        ui->consoleLinkButton->setText(BUTTON_CONNECT);
        ui->consoleLinkButton->setEnabled(true);
        return;
    }

    qDebug() << "Registered with console and listening for events";

    ui->consoleLinkButton->disconnect();
    connect(ui->consoleLinkButton, SIGNAL(clicked()), this, SLOT(consoleDisconnect()));

    ui->consoleLinkLabel->setText(LABEL_CONSOLE_CONNECTED);
    ui->consoleLinkButton->setText(BUTTON_DISCONNECT);
    ui->consoleLinkButton->setEnabled(true);

    // Load the current ontology
    ui->ontologyTreeTitle->setText(QString("Loading..."));
    consoleObserver->loadCurrentOntology();
}

void AnnotationWidget::consoleDisconnect()
{
    QMutexLocker locker(&mutex);

    if (consoleObserverService == NULL) return;

    ui->consoleLinkButton->setText(BUTTON_DISCONNECTING);
    ui->consoleLinkButton->setEnabled(false);
    consoleObserverService->stopServer(); // will delete itself later

    qDebug() << "The consoleObserverService is now is defunct. It will free its own memory within the next"<<CONSOLE_OBSERVER_ACCEPT_TIMEOUT<<"seconds.";
    showDisconnected();
}

void AnnotationWidget::ontologyTreeDoubleClicked(const QModelIndex & index)
{
    EntityTreeItem *item = ontologyTreeModel->node(index);
    if (item->entity() != 0)
    {
        annotateSelectedEntityWithOntologyTerm(item->entity(), item->parent()==0?0:item->parent()->entity());
    }
}

void AnnotationWidget::annotatedBranchTreeClicked(const QModelIndex & index)
{
    EntityTreeItem *item = annotatedBranchTreeModel->node(index);
    if (item->entity() != 0)
    {
        selectedEntity = item->entity();
        emit entitySelected(selectedEntity);
    }
}

bool AnnotationWidget::eventFilter(QObject* watched_object, QEvent* event)
{
    bool filtered = false;
    if (event->type() == QEVENT_KEY_PRESS)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int keyInt = keyEvent->key();
        Qt::Key qtKey = static_cast<Qt::Key>(keyInt);

        // Qt doesn't understand this key code
        if (qtKey == Qt::Key_unknown)
        {
            qDebug() << "Unknown key:"<<qtKey;
            return false;
        }

        // Ignore this event if only a modifier was pressed by itself
        if (qtKey == Qt::Key_Control || qtKey == Qt::Key_Shift || qtKey == Qt::Key_Alt || qtKey == Qt::Key_Meta)
        {
            return false;
        }

        // Create key sequence
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        if (modifiers & Qt::ShiftModifier) keyInt += Qt::SHIFT;
        if (modifiers & Qt::ControlModifier) keyInt += Qt::CTRL;
        if (modifiers & Qt::AltModifier) keyInt += Qt::ALT;
        if (modifiers & Qt::MetaModifier) keyInt += Qt::META;
        QKeySequence keySeq(keyInt);
        qDebug() << "Key sequence:"<<keySeq.toString(QKeySequence::NativeText) << "Portable:"<< keySeq.toString(QKeySequence::PortableText);

        if (ontology != NULL) {
            QMap<QKeySequence, qint64>::const_iterator i = ontology->keyBindMap()->constBegin();
            while (i != ontology->keyBindMap()->constEnd())
            {
                QKeySequence keyBind = i.key();
                if (keyBind.matches(keySeq) == QKeySequence::ExactMatch)
                {
                    Entity *termEntity = ontology->getTermById(i.value());
                    Entity *parentEntity = ontology->getParentById(i.value());
                    ui->ontologyTreeView->selectEntity(*termEntity->id);
                    annotateSelectedEntityWithOntologyTerm(termEntity, parentEntity);
                    filtered = true;
                }
                ++i;
            }
        }
    }
    return filtered;
}

// This reimplements the Console's AnnotateAction
void AnnotationWidget::annotateSelectedEntityWithOntologyTerm(const Entity *term, const Entity *parentTerm)
{
    QMutexLocker locker(&mutex);

    QString termType = term->getValueByAttributeName("Ontology Term Type");

    if (selectedEntity == NULL) return; // Nothing to annotate
    if (termType == "Category" || termType == "Enum") return; // Cannot use these types to annotate
    if (*selectedEntity->entityType == "Annotation") return; // Cannot annotate annotations

    qDebug() << "Annotate"<<(selectedEntity==0?"":*selectedEntity->name)<<"with"<<(term == NULL ? "NULL" : *term->name) << "id="<< *term->id<< "type="<<termType;

    // Get input value, if required
    QString *value = 0;

    if (termType == "Interval")
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Annotating with interval", "Value:", QLineEdit::Normal, "", &ok);

        if (ok && !text.isEmpty())
        {
            ok = false;
            double val = text.toDouble(&ok);
            double lowerBound = term->getValueByAttributeName("Ontology Term Type Interval Lower Bound").toDouble(&ok);
            double upperBound = term->getValueByAttributeName("Ontology Term Type Interval Upper Bound").toDouble(&ok);

            if (!ok || val < lowerBound || val > upperBound) {
                QString msg = QString("Input must be in range: [%1,%2]").arg(lowerBound).arg(upperBound);
                showErrorDialog(msg);
                return;
            }

            value = new QString(text);
        }
    }
    else if (termType == "Text")
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Annotating with text", "Value:", QLineEdit::Normal, "", &ok);

        if (ok && !text.isEmpty())
        {
            value = new QString(text);
        }
    }

    OntologyAnnotation *annotation = new OntologyAnnotation();

    annotation->targetEntityId = new qint64(*selectedEntity->id);
    annotation->keyEntityId = new qint64(*term->id);
    annotation->valueEntityId = 0;
    annotation->keyString = new QString(*term->name);
    annotation->valueString = value;

    if (termType == "EnumItem") {
        if (parentTerm == 0)
        {
            qDebug() << "WARNING: Enum element has no parent! Aborting annotation.";
            return;
        }
        annotation->keyEntityId = new qint64(*parentTerm->id);
        annotation->valueEntityId = new qint64(*term->id);
        annotation->keyString = new QString(*parentTerm->name);
        annotation->valueString = new QString(*term->name);
    }

    if (createAnnotationThread != NULL) createAnnotationThread->disregard();
    createAnnotationThread = new CreateAnnotationThread(annotation);

    connect(createAnnotationThread, SIGNAL(gotResults(const void *)),
            this, SLOT(createAnnotationResults(const void *)));
    connect(createAnnotationThread, SIGNAL(gotError(const QString &)),
            this, SLOT(createAnnotationError(const QString &)));
    createAnnotationThread->start(QThread::NormalPriority);
}

void AnnotationWidget::createAnnotationResults(const void *results)
{
    QMutexLocker locker(&mutex);

    // Succeeded but we can ignore this. The console will fire an annotationsChanged event so that we update the UI.
    delete createAnnotationThread;
    createAnnotationThread = NULL;
}

void AnnotationWidget::createAnnotationError(const QString &error)
{
    QMutexLocker locker(&mutex);

    QString msg = QString("Annotation error: %1").arg(error);
    showErrorDialog(msg);
    delete createAnnotationThread;
    createAnnotationThread = NULL;
}

// This reimplements the Console's EntityTagCloudPanel.deleteTag
void AnnotationWidget::removeAnnotation(const Entity *annotation)
{
    QMutexLocker locker(&mutex);

    if (annotation == NULL) return; // Nothing to annotate
    if (*annotation->entityType != "Annotation") return; // Cannot remove non-annotations

    qDebug() << "Removing Annotation"<<*annotation->name;

    if (removeAnnotationThread != NULL) removeAnnotationThread->disregard();
    removeAnnotationThread = new RemoveAnnotationThread(*annotation->id);

    connect(removeAnnotationThread, SIGNAL(gotResults(const void *)),
            this, SLOT(removeAnnotationResults(const void *)));
    connect(removeAnnotationThread, SIGNAL(gotError(const QString &)),
            this, SLOT(removeAnnotationError(const QString &)));
    removeAnnotationThread->start(QThread::NormalPriority);
}

void AnnotationWidget::removeAnnotationResults(const void *results)
{
    QMutexLocker locker(&mutex);

    // Succeeded but we can ignore this. The console will fire an annotationsChanged event so that we update the UI.
    delete removeAnnotationThread;
    removeAnnotationThread = NULL;
}

void AnnotationWidget::removeAnnotationError(const QString &error)
{
    QMutexLocker locker(&mutex);

    QString msg = QString("Annotation error: %1").arg(error);
    showErrorDialog(msg);
    delete removeAnnotationThread;
    removeAnnotationThread = NULL;
}

void AnnotationWidget::selectEntity(const Entity *entity)
{
    QMutexLocker locker(&mutex);

    selectedEntity = entity;

    if (entity!=0)
        ui->annotatedBranchTreeView->selectEntity(*entity->id);

    emit entitySelected(entity); // This assumes that clients who call selectEntity are smart enough to ignore the subsequent entitySelected signal
}
