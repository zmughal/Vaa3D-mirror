# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'movie_maker_dialog.ui'
#
# Created: Tue Mar  1 08:51:33 2011
#      by: PySide uic UI code generator
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_movie_dialog(object):
    def setupUi(self, movie_dialog):
        movie_dialog.setObjectName("movie_dialog")
        movie_dialog.resize(545, 255)
        self.verticalLayout_2 = QtGui.QVBoxLayout(movie_dialog)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.groupBox = QtGui.QGroupBox(movie_dialog)
        self.groupBox.setObjectName("groupBox")
        self.verticalLayout = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout.setMargin(0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.addCurrentViewButton = QtGui.QPushButton(self.groupBox)
        self.addCurrentViewButton.setDefault(True)
        self.addCurrentViewButton.setObjectName("addCurrentViewButton")
        self.horizontalLayout_2.addWidget(self.addCurrentViewButton)
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setObjectName("label")
        self.horizontalLayout_2.addWidget(self.label)
        self.frameIntervalLineEdit = QtGui.QLineEdit(self.groupBox)
        self.frameIntervalLineEdit.setMaximumSize(QtCore.QSize(50, 16777215))
        self.frameIntervalLineEdit.setText("2.0")
        self.frameIntervalLineEdit.setMaxLength(6)
        self.frameIntervalLineEdit.setObjectName("frameIntervalLineEdit")
        self.horizontalLayout_2.addWidget(self.frameIntervalLineEdit)
        self.label_3 = QtGui.QLabel(self.groupBox)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout_2.addWidget(self.label_3)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        self.keyFrameLabel = QtGui.QLabel(self.groupBox)
        self.keyFrameLabel.setObjectName("keyFrameLabel")
        self.horizontalLayout_2.addWidget(self.keyFrameLabel)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout.addWidget(self.groupBox)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.playButtonBox = QtGui.QDialogButtonBox(movie_dialog)
        self.playButtonBox.setStandardButtons(QtGui.QDialogButtonBox.Help|QtGui.QDialogButtonBox.Reset)
        self.playButtonBox.setCenterButtons(True)
        self.playButtonBox.setObjectName("playButtonBox")
        self.verticalLayout_2.addWidget(self.playButtonBox)
        self.scrollArea = QtGui.QScrollArea(movie_dialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.scrollArea.sizePolicy().hasHeightForWidth())
        self.scrollArea.setSizePolicy(sizePolicy)
        self.scrollArea.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.scrollArea.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName("scrollArea")
        self.scrollAreaWidgetContents = QtGui.QWidget(self.scrollArea)
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 519, 61))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.horizontalLayout_3 = QtGui.QHBoxLayout(self.scrollAreaWidgetContents)
        self.horizontalLayout_3.setContentsMargins(0, 5, 0, 5)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem1)
        self.keyLabelFrame = QtGui.QFrame(self.scrollAreaWidgetContents)
        self.keyLabelFrame.setFrameShape(QtGui.QFrame.NoFrame)
        self.keyLabelFrame.setFrameShadow(QtGui.QFrame.Plain)
        self.keyLabelFrame.setLineWidth(0)
        self.keyLabelFrame.setObjectName("keyLabelFrame")
        self.horizontalLayout_4 = QtGui.QHBoxLayout(self.keyLabelFrame)
        self.horizontalLayout_4.setMargin(0)
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.frameCartoonLabel = QtGui.QLabel(self.keyLabelFrame)
        self.frameCartoonLabel.setMinimumSize(QtCore.QSize(50, 50))
        self.frameCartoonLabel.setMaximumSize(QtCore.QSize(50, 50))
        self.frameCartoonLabel.setFrameShape(QtGui.QFrame.Box)
        self.frameCartoonLabel.setFrameShadow(QtGui.QFrame.Plain)
        self.frameCartoonLabel.setLineWidth(2)
        self.frameCartoonLabel.setObjectName("frameCartoonLabel")
        self.horizontalLayout_4.addWidget(self.frameCartoonLabel)
        self.horizontalLayout_3.addWidget(self.keyLabelFrame)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem2)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.verticalLayout_2.addWidget(self.scrollArea)
        self.buttonBox = QtGui.QDialogButtonBox(movie_dialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Close|QtGui.QDialogButtonBox.Open|QtGui.QDialogButtonBox.Reset|QtGui.QDialogButtonBox.Save|QtGui.QDialogButtonBox.SaveAll)
        self.buttonBox.setObjectName("buttonBox")
        self.verticalLayout_2.addWidget(self.buttonBox)
        self.label.setBuddy(self.frameIntervalLineEdit)
        self.label_3.setBuddy(self.frameIntervalLineEdit)

        self.retranslateUi(movie_dialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("accepted()"), movie_dialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("rejected()"), movie_dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(movie_dialog)

    def retranslateUi(self, movie_dialog):
        movie_dialog.setWindowTitle(QtGui.QApplication.translate("movie_dialog", "V3DCinema Movie Maker", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("movie_dialog", "Key Frames", None, QtGui.QApplication.UnicodeUTF8))
        self.addCurrentViewButton.setText(QtGui.QApplication.translate("movie_dialog", "Add current view", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("movie_dialog", "after", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("movie_dialog", "seconds.  ", None, QtGui.QApplication.UnicodeUTF8))
        self.keyFrameLabel.setText(QtGui.QApplication.translate("movie_dialog", "No key frames added", None, QtGui.QApplication.UnicodeUTF8))
        self.frameCartoonLabel.setText(QtGui.QApplication.translate("movie_dialog", "Frame", None, QtGui.QApplication.UnicodeUTF8))

