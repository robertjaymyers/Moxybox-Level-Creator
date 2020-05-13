#include "CreationScreen.h"

CreationScreen::CreationScreen(QWidget *parent)
	: QGraphicsView(parent)
{
	modLoadThemeIfExists();

	// Modifiable keybinds are initialized before preference load to ensure preference load knows what to look for and edit.
	// (Preference includes storing/loading of keybinds).

	// Grid Visual Aid
	//  _______________________________
	// |Button: (0, 0) | Label: (0, 1) |
	// |_______________________________|
	// |Button: (1, 0) | Label: (1, 1) |
	// |_______________________________|
	// |Button: (2, 0) | Label: (2, 1) |
	// |_______________________________|
	// |Button: (3, 0) | Label: (3, 1) |
	// |_______________________________|

	// Note: First two numbers are row/col in grid of button, second two are row/col in grid of label.
	// (Row is vertical placement, Col is horizontal placement)
	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::MOVE_LEFT,
		keybindComponent{ "Move Left", Qt::Key::Key_A, 0, 0, 0, 1, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::MOVE_RIGHT,
		keybindComponent{ "Move Right", Qt::Key::Key_D, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::MOVE_UP,
		keybindComponent{ "Move Up", Qt::Key::Key_W, 2, 0, 2, 1, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::MOVE_DOWN,
		keybindComponent{ "Move Down", Qt::Key::Key_S, 3, 0, 3, 1, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::OPEN_MENU,
		keybindComponent{ "Open Menu", Qt::Key::Key_Escape, 4, 0, 4, 1, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::PLACE_TOKEN,
		keybindComponent{ "Place Token", Qt::Key::Key_V, 0, 2, 0, 3, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::DELETE_TOKEN,
		keybindComponent{ "Delete Token", Qt::Key::Key_X, 1, 2, 1, 3, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::ROTATE_PATROLLER,
		keybindComponent{ "Rotate Patroller", Qt::Key::Key_R, 2, 2, 2, 3, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::PATROL_RANGE_INC,
		keybindComponent{ "Patrol Range +", Qt::Key::Key_Q, 3, 2, 3, 3, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	keybindMap.insert(std::pair<KeybindModifiable, keybindComponent>(
		KeybindModifiable::PATROL_RANGE_DEC,
		keybindComponent{ "Patrol Range -", Qt::Key::Key_E, 4, 2, 4, 3, Qt::AlignLeft | Qt::AlignTop }
		)
	);

	prefLoad();

	setStyleSheet(styleMap.at("baseStyle"));
	setAlignment(Qt::AlignTop | Qt::AlignLeft);

	scene.get()->setParent(this->parent());
	setScene(scene.get());

	uiGameplayFontGroupTitle.setFamily(standardFontFamily);
	uiGameplayFontGroupTitle.setPointSize(uiGameplayFontGroupTitle_fontPointSize);
	uiGameplayFontGroupTitle.setStyleStrategy(standardFontStyleStrategy);
	uiGameplayFontGroupTitle.setWeight(standardFontWeight);

	uiGameplayFontTextBox.setFamily(standardFontFamily);
	uiGameplayFontTextBox.setPointSize(uiGameplayFontTextBox_fontPointSize);
	uiGameplayFontTextBox.setStyleStrategy(standardFontStyleStrategy);
	uiGameplayFontTextBox.setWeight(standardFontWeight);

	uiGameplayGroup.get()->setGeometry(QRect(0, gridHeight + 1, screenWidth, screenHeight - gridHeight));
	uiGameplayGroup.get()->setLayout(uiGameplayLayout.get());
	uiGameplayGroup.get()->setFlat(true);
	uiGameplayLayout.get()->setMargin(3);
	uiGameplayLayout.get()->addWidget(uiGameplayStatsGroup.get(), 0, 0);
	uiGameplayLayout.get()->addWidget(uiGameplayKeymapGroup.get(), 0, 2);
	uiGameplayLayout.get()->addWidget(uiGameplayMessagesGroup.get(), 0, 1);

	uiGameplayStatsGroup.get()->setCheckable(true);
	uiGameplayStatsGroup.get()->setChecked(false);

	connect(uiGameplayStatsGroup.get(), &QGroupBox::clicked, this, [=]() {
		if (gameState == GameState::CREATING)
		{
			if (uiGameplayStatsGroup.get()->isChecked())
			{
				gameState = GameState::INPUTTING;
			}
		}
		else if (gameState == GameState::INPUTTING)
		{
			if (!uiGameplayStatsGroup.get()->isChecked())
			{
				gameState = GameState::CREATING;
			}
		}
	});

	uiGameplayStatsNameGroup.get()->setLayout(uiGameplayStatsNameLayout.get());
	uiGameplayStatsNameGroup.get()->setStyleSheet(styleMap.at("uiGameplayGroupBoxInnerStyle"));
	uiGameplayStatsNumGroup.get()->setLayout(uiGameplayStatsNumLayout.get());
	uiGameplayStatsNumGroup.get()->setStyleSheet(styleMap.at("uiGameplayGroupBoxInnerStyle"));

	uiGameplayStatsNameLayout.get()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNameLayout.get()->setMargin(0);
	uiGameplayStatsNameLayout.get()->addWidget(uiGameplayStatsLabelCreatorName.get(), 0, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNameLayout.get()->addWidget(uiGameplayStatsInputCreatorName.get(), 1, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNameLayout.get()->addWidget(uiGameplayStatsLabelLevelName.get(), 2, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNameLayout.get()->addWidget(uiGameplayStatsInputLevelName.get(), 3, Qt::AlignLeft | Qt::AlignTop);

	uiGameplayStatsNumLayout.get()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNumLayout.get()->setMargin(0);
	uiGameplayStatsNumLayout.get()->addWidget(uiGameplayStatsLabelLevelTurns.get(), 0, 0, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNumLayout.get()->addWidget(uiGameplayStatsInputLevelTurns.get(), 1, 0, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNumLayout.get()->addWidget(uiGameplayStatsLabelLevelDifficulty.get(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsNumLayout.get()->addWidget(uiGameplayStatsInputLevelDifficulty.get(), 1, 1, Qt::AlignLeft | Qt::AlignTop);

	uiGameplayStatsGroup.get()->setLayout(uiGameplayStatsLayout.get());
	uiGameplayStatsGroup.get()->setMinimumWidth(minBoxWidth);
	uiGameplayStatsLayout.get()->setContentsMargins(5, 25, 0, 5);
	uiGameplayStatsLayout.get()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsLayout.get()->addWidget(uiGameplayStatsNameGroup.get(), 0, 0, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayStatsLayout.get()->addWidget(uiGameplayStatsNumGroup.get(), 1, 0, Qt::AlignLeft | Qt::AlignTop);

	uiGameplayStatsLabelCreatorName.get()->setStyleSheet(styleMap.at("uiGameplayLabelStyle"));
	uiGameplayStatsLabelCreatorName.get()->setFont(uiGameplayFontTextBox);
	uiGameplayStatsLabelCreatorName.get()->setText("Creator Name:");
	uiGameplayStatsInputCreatorName.get()->setStyleSheet(styleMap.at("uiGameplayLineEditStyle"));
	uiGameplayStatsInputCreatorName.get()->setFont(uiGameplayFontTextBox);

	uiGameplayStatsLabelLevelName.get()->setStyleSheet(styleMap.at("uiGameplayLabelStyle"));
	uiGameplayStatsLabelLevelName.get()->setFont(uiGameplayFontTextBox);
	uiGameplayStatsLabelLevelName.get()->setText("Level Name:");
	uiGameplayStatsInputLevelName.get()->setStyleSheet(styleMap.at("uiGameplayLineEditStyle"));
	uiGameplayStatsInputLevelName.get()->setFont(uiGameplayFontTextBox);

	uiGameplayStatsLabelLevelTurns.get()->setStyleSheet(styleMap.at("uiGameplayLabelStyle"));
	uiGameplayStatsLabelLevelTurns.get()->setFont(uiGameplayFontTextBox);
	uiGameplayStatsLabelLevelTurns.get()->setText("Turns:");
	uiGameplayStatsInputLevelTurns.get()->setStyleSheet(styleMap.at("uiGameplayNumEditStyle"));
	uiGameplayStatsInputLevelTurns.get()->setFont(uiGameplayFontTextBox);
	uiGameplayStatsInputLevelTurns.get()->setMaximum(spinBoxMaximum);

	uiGameplayStatsLabelLevelDifficulty.get()->setStyleSheet(styleMap.at("uiGameplayLabelStyle"));
	uiGameplayStatsLabelLevelDifficulty.get()->setFont(uiGameplayFontTextBox);
	uiGameplayStatsLabelLevelDifficulty.get()->setText("Difficulty:");
	uiGameplayStatsInputLevelDifficulty.get()->setStyleSheet(styleMap.at("uiGameplayNumEditStyle"));
	uiGameplayStatsInputLevelDifficulty.get()->setFont(uiGameplayFontTextBox);
	uiGameplayStatsInputLevelDifficulty.get()->setMaximum(spinBoxMaximum);

	uiGameplayMessagesGroup.get()->setLayout(uiGameplayMessagesLayout.get());
	uiGameplayMessagesGroup.get()->setMinimumWidth(minBoxWidth);
	uiGameplayMessagesLayout.get()->setContentsMargins(5, 20, 0, 5);
	uiGameplayMessagesLayout.get()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	uiGameplayMessagesLayout.get()->addWidget(uiGameplayMessagesTextBox.get(), 0, 0, Qt::AlignLeft | Qt::AlignTop);
	uiGameplayMessagesTextBox.get()->setStyleSheet(styleMap.at("uiGameplayTextBoxStyle"));
	uiGameplayMessagesTextBox.get()->setFont(uiGameplayFontTextBox);

	uiGameplayKeymapGroup.get()->setLayout(uiGameplayKeymapLayout.get());
	uiGameplayKeymapGroup.get()->setMinimumWidth(minBoxWidth * 2);
	uiGameplayKeymapLayout.get()->setContentsMargins(5, 20, 0, 5);
	uiGameplayKeymapLayout.get()->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	for (auto& k : keybindMap)
	{
		uiGameplayKeymapLayout.get()->addWidget
		(
			k.second.uiButton.get(),
			k.second.uiButtonGridLayoutRow,
			k.second.uiButtonGridLayoutCol,
			k.second.gridLayoutAlign
		);

		uiGameplayKeymapLayout.get()->addWidget
		(
			k.second.uiLabel.get(),
			k.second.uiLabelGridLayoutRow,
			k.second.uiLabelGridLayoutCol,
			k.second.gridLayoutAlign
		);

		k.second.uiButton.get()->setMinimumSize(QSize(uiGameplayKeymapBtnSize, uiGameplayKeymapBtnSize));
		k.second.uiButton.get()->setStyleSheet(styleMap.at("uiGameplayKeymapBtnStyle"));
		k.second.uiButton.get()->setFont(uiGameplayFontTextBox);
		k.second.uiButton.get()->setText(QKeySequence(k.second.keybind).toString());

		k.second.uiLabel.get()->setMinimumHeight(uiGameplayKeymapBtnSize);
		k.second.uiLabel.get()->setStyleSheet(styleMap.at("uiGameplayLabelStyle"));
		k.second.uiLabel.get()->setFont(uiGameplayFontTextBox);
		k.second.uiLabel.get()->setText(k.second.labelText);

		connect(k.second.uiButton.get(), &QPushButton::clicked, this, [&]() {
			if (gameState == GameState::CREATING)
			{
				uiGameplayKeymapGroup.get()->setTitle(uiGameplayKeymapGroupTitleKeybinding);
				k.second.uiButton.get()->setStyleSheet(styleMap.at("uiGameplayKeymapModifBtnStyle"));
				keybindToModify = k.first;
				gameState = GameState::KEYBINDING;
			}
			else if (gameState == GameState::KEYBINDING)
			{
				uiGameplayKeymapGroup.get()->setTitle(uiGameplayKeymapGroupTitleDefault);
				keybindMap.at(keybindToModify).uiButton.get()->setStyleSheet(styleMap.at("uiGameplayKeymapBtnStyle"));
				keybindToModify = KeybindModifiable::NONE;
				gameState = GameState::CREATING;
			}
		});
	}

	uiGameplayStatsGroup.get()->setFont(uiGameplayFontGroupTitle);
	uiGameplayMessagesGroup.get()->setFont(uiGameplayFontGroupTitle);
	uiGameplayKeymapGroup.get()->setFont(uiGameplayFontGroupTitle);

	uiGameplayStatsGroup.get()->setStyleSheet(styleMap.at("uiGameplayGroupBoxStyle"));
	uiGameplayMessagesGroup.get()->setStyleSheet(styleMap.at("uiGameplayGroupBoxStyle"));
	uiGameplayKeymapGroup.get()->setStyleSheet(styleMap.at("uiGameplayGroupBoxStyle"));

	uiGameplayStatsGroup.get()->setTitle("Stats");
	uiGameplayMessagesGroup.get()->setTitle("Messages");
	uiGameplayKeymapGroup.get()->setTitle(uiGameplayKeymapGroupTitleDefault);

	uiMenuFontBtn.setFamily(standardFontFamily);
	uiMenuFontBtn.setPointSize(uiMenuFontBtn_fontPointSize);
	uiMenuFontBtn.setStyleStrategy(standardFontStyleStrategy);
	uiMenuFontBtn.setWeight(standardFontWeight);

	uiMenuGroup.get()->setGeometry(QRect(
		(screenWidth / 2) - (uiMenuWidth / 2),
		(screenHeight / 2) - (uiMenuHeight / 2),
		uiMenuWidth,
		uiMenuHeight)
	);
	uiMenuGroup.get()->setLayout(uiMenuLayout.get());
	uiMenuGroup.get()->setFlat(true);
	uiMenuGroup.get()->setStyleSheet(styleMap.at("uiMenuGroupBoxStyle"));
	uiMenuLayout.get()->setMargin(0);
	uiMenuLayout.get()->setVerticalSpacing(uiMenuBtnSpacing);
	uiMenuLayout.get()->setAlignment(Qt::AlignTop);

	uiMenuBtnMap.insert(std::pair<UiMenuBtnType, uiMenuBtnComponent>(
		UiMenuBtnType::RESUME,
		uiMenuBtnComponent{ "RESUME", 0, 0, Qt::AlignTop }
		)
	);

	uiMenuBtnMap.insert(std::pair<UiMenuBtnType, uiMenuBtnComponent>(
		UiMenuBtnType::SAVE,
		uiMenuBtnComponent{ "SAVE", 1, 0, Qt::AlignTop }
		)
	);

	uiMenuBtnMap.insert(std::pair<UiMenuBtnType, uiMenuBtnComponent>(
		UiMenuBtnType::SAVE_AS,
		uiMenuBtnComponent{ "SAVE AS...", 2, 0, Qt::AlignTop }
		)
	);

	uiMenuBtnMap.insert(std::pair<UiMenuBtnType, uiMenuBtnComponent>(
		UiMenuBtnType::LOAD,
		uiMenuBtnComponent{ "OPEN...", 3, 0, Qt::AlignTop }
		)
	);

	uiMenuBtnMap.insert(std::pair<UiMenuBtnType, uiMenuBtnComponent>(
		UiMenuBtnType::NEW,
		uiMenuBtnComponent{ "NEW", 4, 0, Qt::AlignTop }
		)
	);

	uiMenuBtnMap.insert(std::pair<UiMenuBtnType, uiMenuBtnComponent>(
		UiMenuBtnType::EXIT,
		uiMenuBtnComponent{ "EXIT GAME", 5, 0, Qt::AlignTop }
		)
	);

	for (auto& component : uiMenuBtnMap)
	{
		uiMenuLayout.get()->addWidget
		(
			component.second.widget.get(),
			component.second.gridLayoutRow,
			component.second.gridLayoutCol,
			component.second.gridLayoutAlign
		);

		component.second.widget.get()->setFixedSize(QSize(uiMenuBtnWidth, uiMenuBtnHeight));
		component.second.widget.get()->setStyleSheet(styleMap.at("uiMenuBtnStyle"));
		component.second.widget.get()->setFont(uiMenuFontBtn);
		component.second.widget.get()->setText(component.second.text);

		switch (component.first)
		{
		case UiMenuBtnType::RESUME:
			connect(uiMenuBtnMap.at(component.first).widget.get(), &QPushButton::clicked, this, &CreationScreen::unpauseProgram);
			break;
		case UiMenuBtnType::SAVE:
			connect(uiMenuBtnMap.at(component.first).widget.get(), &QPushButton::clicked, this, &CreationScreen::fileSaveOperationType);
			break;
		case UiMenuBtnType::SAVE_AS:
			connect(uiMenuBtnMap.at(component.first).widget.get(), &QPushButton::clicked, this, &CreationScreen::fileSaveAs);
			break;
		case UiMenuBtnType::LOAD:
			connect(uiMenuBtnMap.at(component.first).widget.get(), &QPushButton::clicked, this, &CreationScreen::fileLoad);
			break;
		case UiMenuBtnType::NEW:
			connect(uiMenuBtnMap.at(component.first).widget.get(), &QPushButton::clicked, this, &CreationScreen::fileNew);
			break;
		case UiMenuBtnType::EXIT:
			connect(uiMenuBtnMap.at(component.first).widget.get(), &QPushButton::clicked, this, &CreationScreen::exitProgram);
			break;
		}
	}

	uiMenuGroup.get()->setVisible(false);

	// LEVEL MODIFIED SIGNAL CHECKS:
	// Note: Because these boxes are modified on initial program load with defaults, 
	// it's going to trigger a false positive of the level being modified. To get around this, 
	// we need to make sure to set the level to unmodified after everything is initially loaded.
	connect(uiGameplayStatsInputCreatorName.get(), &QLineEdit::textChanged, this, [this] { setLevelModified(true); });
	connect(uiGameplayStatsInputLevelName.get(), &QLineEdit::textChanged, this, [this] { setLevelModified(true); });
	connect(uiGameplayStatsInputLevelTurns.get(), QOverload<int>::of(&QSpinBox::valueChanged), this, [this] { setLevelModified(true); });
	connect(uiGameplayStatsInputLevelDifficulty.get(), QOverload<int>::of(&QSpinBox::valueChanged), this, [this] { setLevelModified(true); });

	// GRID CORNER
	gridPiecesCorner.reserve(4);

	gridPieceCorner cornerUpL;
	cornerUpL.item.get()->setPixmap(imgMap.at("imgGridCornerUpL"));
	cornerUpL.item.get()->setPos(gridAnchorX, gridAnchorY);
	cornerUpL.facing = gridPieceCorner::Facing::UP_LEFT;
	gridPiecesCorner.emplace_back(std::move(cornerUpL));

	gridPieceCorner cornerUpR;
	cornerUpR.item.get()->setPixmap(imgMap.at("imgGridCornerUpR"));
	cornerUpR.item.get()->setPos(gridBoundRight - gridPieceSize, gridAnchorY);
	cornerUpR.facing = gridPieceCorner::Facing::UP_RIGHT;
	gridPiecesCorner.emplace_back(std::move(cornerUpR));

	gridPieceCorner cornerDownL;
	cornerDownL.item.get()->setPixmap(imgMap.at("imgGridCornerDownL"));
	cornerDownL.item.get()->setPos(gridAnchorX, gridBoundDown - gridPieceSize);
	cornerDownL.facing = gridPieceCorner::Facing::DOWN_LEFT;
	gridPiecesCorner.emplace_back(std::move(cornerDownL));

	gridPieceCorner cornerDownR;
	cornerDownR.item.get()->setPixmap(imgMap.at("imgGridCornerDownR"));
	cornerDownR.item.get()->setPos(gridBoundRight - gridPieceSize, gridBoundDown - gridPieceSize);
	cornerDownR.facing = gridPieceCorner::Facing::DOWN_RIGHT;
	gridPiecesCorner.emplace_back(std::move(cornerDownR));

	for (auto& piece : gridPiecesCorner)
	{
		piece.item.get()->setZValue(gridPieceZ);
		scene.get()->addItem(piece.item.get());
	}


	// GRID INNER
	const int gridInnerXConst = gridAnchorX + gridPieceSize;
	int gridInnerY = gridAnchorY + gridPieceSize;
	int gridInnerX = gridInnerXConst;
	const int downInnerTotal = gridColSize - 2;
	const int acrossInnerTotal = gridRowSize - 2;
	gridPiecesInner.reserve(downInnerTotal * acrossInnerTotal);
	for (int downInner = 0; downInner < downInnerTotal; downInner++)
	{
		for (int acrossInner = 0; acrossInner < acrossInnerTotal; acrossInner++)
		{
			gridPieceInner piece;
			piece.item.get()->setPixmap(imgMap.at("imgGridInner"));
			piece.item.get()->setPos(gridInnerX, gridInnerY);
			gridPiecesInner.emplace_back(std::move(piece));

			gridInnerX += gridPieceSize;
		}
		gridInnerY += gridPieceSize;
		gridInnerX = gridInnerXConst;
	}

	for (auto& piece : gridPiecesInner)
	{
		piece.item.get()->setZValue(gridPieceZ);
		scene.get()->addItem(piece.item.get());
	}


	// GRID EDGE
	const int numEdgesHorizontal = gridRowSize - 2;
	const int numEdgesVertical = gridColSize - 2;
	gridPiecesEdge.reserve(numEdgesHorizontal * numEdgesVertical);

	// Edge naming convention: X and Y are the directions the tiles are placed in.
	// i.e. UpX is the tiles placed across, at the top.

	int gridPieceEdgeUpX = gridPieceSize + gridAnchorX;
	for (int i = 0; i < numEdgesHorizontal; i++)
	{
		gridPieceEdge piece;
		piece.item.get()->setPixmap(imgMap.at("imgGridEdgeUpX"));
		piece.item.get()->setPos(gridPieceEdgeUpX, gridAnchorY);
		piece.facing = gridPieceEdge::Facing::UP;
		gridPiecesEdge.emplace_back(std::move(piece));

		gridPieceEdgeUpX += gridPieceSize;
	}

	int gridPieceEdgeDownX = gridPieceSize + gridAnchorX;
	for (int i = 0; i < numEdgesHorizontal; i++)
	{
		gridPieceEdge piece;
		piece.item.get()->setPixmap(imgMap.at("imgGridEdgeDownX"));
		piece.item.get()->setPos(gridPieceEdgeDownX, gridBoundDown - gridPieceSize);
		piece.facing = gridPieceEdge::Facing::DOWN;
		gridPiecesEdge.emplace_back(std::move(piece));

		gridPieceEdgeDownX += gridPieceSize;
	}

	int gridPieceEdgeLeftY = gridAnchorY + gridPieceSize;
	for (int i = 0; i < numEdgesVertical; i++)
	{
		gridPieceEdge piece;
		piece.item.get()->setPixmap(imgMap.at("imgGridEdgeLeftY"));
		piece.item.get()->setPos(gridAnchorX, gridPieceEdgeLeftY);
		piece.facing = gridPieceEdge::Facing::LEFT;
		gridPiecesEdge.emplace_back(std::move(piece));

		gridPieceEdgeLeftY += gridPieceSize;
	}

	int gridPieceEdgeRightY = gridAnchorY + gridPieceSize;
	for (int i = 0; i < numEdgesVertical; i++)
	{
		gridPieceEdge piece;
		piece.item.get()->setPixmap(imgMap.at("imgGridEdgeRightY"));
		piece.item.get()->setPos(gridBoundRight - gridPieceSize, gridPieceEdgeRightY);
		piece.facing = gridPieceEdge::Facing::RIGHT;
		gridPiecesEdge.emplace_back(std::move(piece));

		gridPieceEdgeRightY += gridPieceSize;
	}

	for (auto& piece : gridPiecesEdge)
	{
		piece.item.get()->setZValue(gridPieceZ);
		scene.get()->addItem(piece.item.get());
	}

	highlightToken.get()->setPixmap(imgMap.at("imgTokenPlacingOutline"));
	highlightToken.get()->setPos(highlightOffsetX, highlightOffsetY);

	scene.get()->addItem(highlightToken.get());

	fadedToken.get()->setOpacity(0.33);
	fadedToken.get()->setPixmap(tokenTypeToImg(tokenType));
	updateFadedTokenPosition();
	scene.get()->addItem(fadedToken.get());

	// We make a list of scene items for displaying patrol path based on the longest it could be,
	// using the highest size of the grid in number of squares - 1, since 1 will be occupied
	// by patroller.
	// For example, with patrol being only VERTICAL or HORIZONTAL and token taking up a square,
	// the highest a patrol path could be is 19 squares, given highest number of squares in a single direction as 20.
	// This number is an aesthetic design choice more than anything else. It has no impact on what patrol
	// points can be increased or decreased to. It's purely for displaying to the user where a 
	// patroller will patrol to, when they are highlighted over that square on the grid.
	// If patrol paths were to be redesigned so that they can shift between VERTICAL and HORIZONTAL
	// to form a more complex path, the design here would need to be adapted to that to accommodate.
	// For example, we could make a vector of the size of the total number of squares on the board - 1.
	// Either way, we need to be able to display as images on squares in the scene to indicate
	// where the patrol path goes.
	for (int i = 0; i < gridRowSize - 1; i++)
	{
		patrollerHoverItems.emplace_back(std::make_unique<QGraphicsPixmapItem>(nullptr));
	}

	for (auto& item : patrollerHoverItems)
	{
		item.get()->setZValue(patrollerHoverZValue);
		scene.get()->addItem(item.get());
	}

	uiGameplaySetToDefaults();
	setLevelModified(false);
	gameState = GameState::CREATING;
}

// protected:

void CreationScreen::wheelEvent(QWheelEvent *event)
{
	if (gameState == GameState::CREATING)
	{
		if (event->angleDelta().y() > 0)
		{
			// forward
			qDebug("forward");

			if (fadedNum == fadedMin)
				fadedNum = fadedMax;
			else
				fadedNum--;

			tokenType = tokenTypeList[fadedNum];
			fadedToken->setPixmap(tokenTypeToImg(tokenType));
		}
		else if (event->angleDelta().y() < 0)
		{
			// back
			qDebug("back");

			if (fadedNum == fadedMax)
				fadedNum = fadedMin;
			else
				fadedNum++;

			tokenType = tokenTypeList[fadedNum];
			fadedToken->setPixmap(tokenTypeToImg(tokenType));
		}
		event->accept();
	}
}

void CreationScreen::keyReleaseEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		if (gameState == GameState::CREATING)
		{
			if (event->key() == keybindMap.at(KeybindModifiable::MOVE_LEFT).keybind)
			{
				uiGameplayMessagesTextBox.get()->setText("");

				if (highlightToken.get()->x() - (gridPieceSize * highlightMovementSpeed) > gridBoundLeft)
				{
					highlightToken.get()->setPos
					(
						highlightToken.get()->x() - (gridPieceSize * highlightMovementSpeed),
						highlightToken.get()->y()
					);
					updateFadedTokenPosition();

					QString fadedPosStr =
						QString::number(fadedToken.get()->x()) +
						QString::number(fadedToken.get()->y());

					std::map<QString, tokenPatroller>::iterator pusherIt;
					pusherIt = pushersMap.find(fadedPosStr);

					std::map<QString, tokenPatroller>::iterator suckerIt;
					suckerIt = suckersMap.find(fadedPosStr);

					if (pusherIt != pushersMap.end())
					{
						showPatrollerHoverDisplay(pusherIt);
					}
					else if (suckerIt != suckersMap.end())
					{
						showPatrollerHoverDisplay(suckerIt);
					}
					else
					{
						if (patrollerHovering)
						{
							hidePatrollerHoverDisplay();
						}
					}
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::MOVE_RIGHT).keybind)
			{
				uiGameplayMessagesTextBox.get()->setText("");

				if (highlightToken.get()->x() + (gridPieceSize * highlightMovementSpeed) < gridBoundRight)
				{
					highlightToken.get()->setPos
					(
						highlightToken.get()->x() + (gridPieceSize * highlightMovementSpeed),
						highlightToken.get()->y()
					);
					updateFadedTokenPosition();

					QString fadedPosStr =
						QString::number(fadedToken.get()->x()) +
						QString::number(fadedToken.get()->y());

					std::map<QString, tokenPatroller>::iterator pusherIt;
					pusherIt = pushersMap.find(fadedPosStr);

					std::map<QString, tokenPatroller>::iterator suckerIt;
					suckerIt = suckersMap.find(fadedPosStr);

					if (pusherIt != pushersMap.end())
					{
						showPatrollerHoverDisplay(pusherIt);
					}
					else if (suckerIt != suckersMap.end())
					{
						showPatrollerHoverDisplay(suckerIt);
					}
					else
					{
						if (patrollerHovering)
						{
							hidePatrollerHoverDisplay();
						}
					}
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::MOVE_UP).keybind)
			{
				uiGameplayMessagesTextBox.get()->setText("");

				if (highlightToken.get()->y() - (gridPieceSize * highlightMovementSpeed) > gridBoundUp)
				{
					highlightToken.get()->setPos
					(
						highlightToken.get()->x(),
						highlightToken.get()->y() - (gridPieceSize * highlightMovementSpeed)
					);
					updateFadedTokenPosition();

					QString fadedPosStr =
						QString::number(fadedToken.get()->x()) +
						QString::number(fadedToken.get()->y());

					std::map<QString, tokenPatroller>::iterator pusherIt;
					pusherIt = pushersMap.find(fadedPosStr);

					std::map<QString, tokenPatroller>::iterator suckerIt;
					suckerIt = suckersMap.find(fadedPosStr);

					if (pusherIt != pushersMap.end())
					{
						showPatrollerHoverDisplay(pusherIt);
					}
					else if (suckerIt != suckersMap.end())
					{
						showPatrollerHoverDisplay(suckerIt);
					}
					else
					{
						if (patrollerHovering)
						{
							hidePatrollerHoverDisplay();
						}
					}
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::MOVE_DOWN).keybind)
			{
				uiGameplayMessagesTextBox.get()->setText("");

				if (highlightToken.get()->y() + (gridPieceSize * highlightMovementSpeed) < gridBoundDown)
				{
					highlightToken.get()->setPos
					(
						highlightToken.get()->x(),
						highlightToken.get()->y() + (gridPieceSize * highlightMovementSpeed)
					);
					updateFadedTokenPosition();

					QString fadedPosStr =
						QString::number(fadedToken.get()->x()) +
						QString::number(fadedToken.get()->y());

					std::map<QString, tokenPatroller>::iterator pusherIt;
					pusherIt = pushersMap.find(fadedPosStr);

					std::map<QString, tokenPatroller>::iterator suckerIt;
					suckerIt = suckersMap.find(fadedPosStr);

					if (pusherIt != pushersMap.end())
					{
						showPatrollerHoverDisplay(pusherIt);
					}
					else if (suckerIt != suckersMap.end())
					{
						showPatrollerHoverDisplay(suckerIt);
					}
					else
					{
						if (patrollerHovering)
						{
							hidePatrollerHoverDisplay();
						}
					}
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::PLACE_TOKEN).keybind)
			{
				if (tokenType != TokenType::NONE)
				{
					QString fadedPos =
						QString::number(fadedToken.get()->x()) +
						QString::number(fadedToken.get()->y());

					if (playersMap.count(fadedPos) > 0 ||
						pushersMap.count(fadedPos) > 0 ||
						suckersMap.count(fadedPos) > 0 ||
						blocksMap.count(fadedPos) > 0 ||
						keysMap.count(fadedPos) > 0 ||
						gatesMap.count(fadedPos) > 0 ||
						hazardsMap.count(fadedPos) > 0 ||
						utilsMap.count(fadedPos) > 0)
					{
						uiGameplayMessagesTextBox.get()->setText(uiGameplayMessagesSquareOccupied);
					}
					else if (tokenType == TokenType::PLAYER && !playersMap.empty())
					{
						uiGameplayMessagesTextBox.get()->setText(uiGameplayMessagesPlayerAlreadyPlaced);
					}
					else
					{
						switch (tokenType)
						{
						case TokenType::PLAYER:
							playersMap.insert
							(
								std::pair<QString, tokenPlayer>
								(
									fadedPos,
									tokenPlayer
									{
										fadedToken.get()->x(),
										fadedToken.get()->y()
									}
								)
							);
							scene.get()->addItem(playersMap.at(fadedPos).item.get());
							playersMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgPlayer"));
							playersMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						case TokenType::PUSHER:
							pushersMap.insert
							(
								std::pair<QString, tokenPatroller>
								(
									fadedPos,
									tokenPatroller
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenPatroller::Type::PUSHER
									}
								)
							);
							scene.get()->addItem(pushersMap.at(fadedPos).item.get());
							pushersMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgPatrollerPusherUp"));
							pushersMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							showPatrollerHoverDisplay(pushersMap, fadedPos);
							break;
						case TokenType::SUCKER:
							suckersMap.insert
							(
								std::pair<QString, tokenPatroller>
								(
									fadedPos,
									tokenPatroller
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenPatroller::Type::SUCKER
									}
								)
							);
							scene.get()->addItem(suckersMap.at(fadedPos).item.get());
							suckersMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgPatrollerSuckerUp"));
							suckersMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							showPatrollerHoverDisplay(suckersMap, fadedPos);
							break;
						case TokenType::BLOCK:
							blocksMap.insert
							(
								std::pair<QString, tokenImmobile>
								(
									fadedPos,
									tokenImmobile
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenImmobile::Type::BLOCK
									}
								)
							);
							scene.get()->addItem(blocksMap.at(fadedPos).item.get());
							blocksMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgImmobileBlock"));
							blocksMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						case TokenType::KEY:
							keysMap.insert
							(
								std::pair<QString, tokenImmobile>
								(
									fadedPos,
									tokenImmobile
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenImmobile::Type::KEY
									}
								)
							);
							scene.get()->addItem(keysMap.at(fadedPos).item.get());
							keysMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgImmobileKey"));
							keysMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						case TokenType::GATE:
							gatesMap.insert
							(
								std::pair<QString, tokenImmobile>
								(
									fadedPos,
									tokenImmobile
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenImmobile::Type::GATE
									}
								)
							);
							scene.get()->addItem(gatesMap.at(fadedPos).item.get());
							gatesMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgImmobileGate"));
							gatesMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						case TokenType::HAZARD:
							hazardsMap.insert
							(
								std::pair<QString, tokenImmobile>
								(
									fadedPos,
									tokenImmobile
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenImmobile::Type::HAZARD
									}
								)
							);
							scene.get()->addItem(hazardsMap.at(fadedPos).item.get());
							hazardsMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgImmobileHazard"));
							hazardsMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						case TokenType::UTIL_PUSH:
							utilsMap.insert
							(
								std::pair<QString, tokenUtil>
								(
									fadedPos,
									tokenUtil
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenUtil::Type::PUSHER,
										tokenUtil::State::INACTIVE
									}
								)
							);
							scene.get()->addItem(utilsMap.at(fadedPos).item.get());
							utilsMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgUtilPusherInactive"));
							utilsMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						case TokenType::UTIL_SUCK:
							utilsMap.insert
							(
								std::pair<QString, tokenUtil>
								(
									fadedPos,
									tokenUtil
									{
										fadedToken.get()->x(),
										fadedToken.get()->y(),
										tokenUtil::Type::SUCKER,
										tokenUtil::State::INACTIVE
									}
								)
							);
							scene.get()->addItem(utilsMap.at(fadedPos).item.get());
							utilsMap.at(fadedPos).item.get()->setPixmap(imgMap.at("imgUtilSuckerInactive"));
							utilsMap.at(fadedPos).item.get()->setPos(fadedToken.get()->x(), fadedToken.get()->y());
							break;
						}
						uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(tokenType) + uiGameplayMessagesTokenPlaced);
						setLevelModified(true);
					}
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::DELETE_TOKEN).keybind)
			{
				QString fadedPos =
					QString::number(fadedToken.get()->x()) +
					QString::number(fadedToken.get()->y());

				int fadedX = fadedToken.get()->x();
				int fadedY = fadedToken.get()->y();

				if (playersMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(playersMap.at(fadedPos).item.get());
					playersMap.erase(fadedPos);
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::PLAYER) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (pushersMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(pushersMap.at(fadedPos).item.get());
					pushersMap.erase(fadedPos);
					hidePatrollerHoverDisplay();
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::PUSHER) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (suckersMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(suckersMap.at(fadedPos).item.get());
					suckersMap.erase(fadedPos);
					hidePatrollerHoverDisplay();
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::SUCKER) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (blocksMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(blocksMap.at(fadedPos).item.get());
					blocksMap.erase(fadedPos);
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::BLOCK) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (keysMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(keysMap.at(fadedPos).item.get());
					keysMap.erase(fadedPos);
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::KEY) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (gatesMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(gatesMap.at(fadedPos).item.get());
					gatesMap.erase(fadedPos);
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::GATE) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (hazardsMap.count(fadedPos) > 0)
				{
					scene.get()->removeItem(hazardsMap.at(fadedPos).item.get());
					hazardsMap.erase(fadedPos);
					uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::HAZARD) + uiGameplayMessagesTokenDeleted);
					setLevelModified(true);
				}
				else if (utilsMap.count(fadedPos) > 0)
				{
					std::map<QString, tokenUtil>::iterator it;
					it = utilsMap.find(fadedPos);
					if (it->second.type == tokenUtil::Type::PUSHER)
						uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::UTIL_PUSH) + uiGameplayMessagesTokenDeleted);
					else if (it->second.type == tokenUtil::Type::SUCKER)
						uiGameplayMessagesTextBox.get()->setText(tokenDisplayNameMap.at(TokenType::UTIL_SUCK) + uiGameplayMessagesTokenDeleted);
					scene.get()->removeItem(utilsMap.at(fadedPos).item.get());
					utilsMap.erase(fadedPos);
					setLevelModified(true);
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::ROTATE_PATROLLER).keybind)
			{
				QString fadedPosStr =
					QString::number(fadedToken.get()->x()) +
					QString::number(fadedToken.get()->y());

				std::map<QString, tokenPatroller>::iterator pusherIt;
				pusherIt = pushersMap.find(fadedPosStr);

				std::map<QString, tokenPatroller>::iterator suckerIt;
				suckerIt = suckersMap.find(fadedPosStr);

				if (pusherIt != pushersMap.end())
				{
					rotatePatrollerFacingClockwise(pusherIt);
					showPatrollerHoverDisplay(pusherIt);
					pusherIt->second.item.get()->setPixmap(facingToImg(pusherIt->second.facing, pusherIt->second.type));
					setLevelModified(true);
				}
				else if (suckerIt != suckersMap.end())
				{
					rotatePatrollerFacingClockwise(suckerIt);
					showPatrollerHoverDisplay(suckerIt);
					suckerIt->second.item.get()->setPixmap(facingToImg(suckerIt->second.facing, suckerIt->second.type));
					setLevelModified(true);
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::PATROL_RANGE_INC).keybind)
			{
				QString fadedPosStr =
					QString::number(fadedToken.get()->x()) +
					QString::number(fadedToken.get()->y());

				std::map<QString, tokenPatroller>::iterator pusherIt;
				pusherIt = pushersMap.find(fadedPosStr);

				std::map<QString, tokenPatroller>::iterator suckerIt;
				suckerIt = suckersMap.find(fadedPosStr);

				if (pusherIt != pushersMap.end())
				{
					switch (pusherIt->second.facing)
					{
					case tokenPatroller::Facing::LEFT:
						pusherIt->second.patrolBoundLeft++;
						break;
					case tokenPatroller::Facing::RIGHT:
						pusherIt->second.patrolBoundRight++;
						break;
					case tokenPatroller::Facing::UP:
						pusherIt->second.patrolBoundUp++;
						break;
					case tokenPatroller::Facing::DOWN:
						pusherIt->second.patrolBoundDown++;
						break;
					}
					showPatrollerHoverDisplay(pusherIt);
					setLevelModified(true);
				}
				else if (suckerIt != suckersMap.end())
				{
					switch (suckerIt->second.facing)
					{
					case tokenPatroller::Facing::LEFT:
						suckerIt->second.patrolBoundLeft++;
						break;
					case tokenPatroller::Facing::RIGHT:
						suckerIt->second.patrolBoundRight++;
						break;
					case tokenPatroller::Facing::UP:
						suckerIt->second.patrolBoundUp++;
						break;
					case tokenPatroller::Facing::DOWN:
						suckerIt->second.patrolBoundDown++;
						break;
					}
					showPatrollerHoverDisplay(suckerIt);
					setLevelModified(true);
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::PATROL_RANGE_DEC).keybind)
			{
				QString fadedPosStr =
					QString::number(fadedToken.get()->x()) +
					QString::number(fadedToken.get()->y());

				std::map<QString, tokenPatroller>::iterator pusherIt;
				pusherIt = pushersMap.find(fadedPosStr);

				std::map<QString, tokenPatroller>::iterator suckerIt;
				suckerIt = suckersMap.find(fadedPosStr);

				if (pusherIt != pushersMap.end())
				{
					switch (pusherIt->second.facing)
					{
					case tokenPatroller::Facing::LEFT:
						if (pusherIt->second.patrolBoundLeft - 1 >= 0)
							pusherIt->second.patrolBoundLeft--;
						break;
					case tokenPatroller::Facing::RIGHT:
						if (pusherIt->second.patrolBoundRight - 1 >= 0)
							pusherIt->second.patrolBoundRight--;
						break;
					case tokenPatroller::Facing::UP:
						if (pusherIt->second.patrolBoundUp - 1 >= 0)
							pusherIt->second.patrolBoundUp--;
						break;
					case tokenPatroller::Facing::DOWN:
						if (pusherIt->second.patrolBoundDown - 1 >= 0)
							pusherIt->second.patrolBoundDown--;
						break;
					}
					showPatrollerHoverDisplay(pusherIt);
					setLevelModified(true);
				}
				else if (suckerIt != suckersMap.end())
				{
					switch (suckerIt->second.facing)
					{
					case tokenPatroller::Facing::LEFT:
						if (suckerIt->second.patrolBoundLeft - 1 >= 0)
							suckerIt->second.patrolBoundLeft--;
						break;
					case tokenPatroller::Facing::RIGHT:
						if (suckerIt->second.patrolBoundRight - 1 >= 0)
							suckerIt->second.patrolBoundRight--;
						break;
					case tokenPatroller::Facing::UP:
						if (suckerIt->second.patrolBoundUp - 1 >= 0)
							suckerIt->second.patrolBoundUp--;
						break;
					case tokenPatroller::Facing::DOWN:
						if (suckerIt->second.patrolBoundDown - 1 >= 0)
							suckerIt->second.patrolBoundDown--;
						break;
					}
					showPatrollerHoverDisplay(suckerIt);
					setLevelModified(true);
				}
			}
			else if (event->key() == keybindMap.at(KeybindModifiable::OPEN_MENU).keybind)
			{
				gameState = GameState::PAUSED;
				uiMenuGroup.get()->setVisible(true);
				uiGameplayStatsGroup.get()->setEnabled(false);
			}
		}
		else if (gameState == GameState::PAUSED)
		{
			if (event->key() == keybindMap.at(KeybindModifiable::OPEN_MENU).keybind)
			{
				uiMenuResumeCreating();
			}
		}
		else if (gameState == GameState::KEYBINDING)
		{
			Qt::Key newKeybind = Qt::Key(event->key());

			// check if keybind is equal to an existing keybind that isn't the one being changed
			// if it is, set that keybind to nothing? or warn player.
			if (newKeybind == keybindMap.at(KeybindModifiable::MOVE_LEFT).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::MOVE_RIGHT).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::MOVE_UP).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::MOVE_DOWN).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::OPEN_MENU).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::PLACE_TOKEN).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::DELETE_TOKEN).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::ROTATE_PATROLLER).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::PATROL_RANGE_INC).keybind ||
				newKeybind == keybindMap.at(KeybindModifiable::PATROL_RANGE_DEC).keybind)
			{
				for (const auto& k : keybindMap)
				{
					if (newKeybind == k.second.keybind)
					{
						QMessageBox qMsg(this->parentWidget());
						qMsg.setStyleSheet(styleMap.at("uiMessageBoxStyle"));
						qMsg.setWindowTitle("Keybind In Use");
						qMsg.setText("\"" + QKeySequence(newKeybind).toString() + "\" is already bound to the \"" + k.second.labelText + "\" command.");
						qMsg.setStandardButtons(QMessageBox::Ok);
						qMsg.setDefaultButton(QMessageBox::Ok);
						qMsg.setFont(uiGameplayFontTextBox);
						qMsg.button(QMessageBox::Ok)->setFont(uiGameplayFontTextBox);
						qMsg.exec();

						return;
					}
				}
			}
			else if (newKeybind == Qt::Key_Control || newKeybind == Qt::Key_Shift)
			{
				QMessageBox qMsg(this->parentWidget());
				qMsg.setStyleSheet(styleMap.at("uiMessageBoxStyle"));
				qMsg.setWindowTitle("Keybind Invalid");
				qMsg.setText("Ctrl and Shift are not valid key input.");
				qMsg.setStandardButtons(QMessageBox::Ok);
				qMsg.setDefaultButton(QMessageBox::Ok);
				qMsg.setFont(uiGameplayFontTextBox);
				qMsg.button(QMessageBox::Ok)->setFont(uiGameplayFontTextBox);
				qMsg.exec();
			}
			else
			{
				keybindMap.at(keybindToModify).keybind = newKeybind;
				keybindMap.at(keybindToModify).uiButton.get()->setStyleSheet(styleMap.at("uiGameplayKeymapBtnStyle"));
				keybindMap.at(keybindToModify).uiButton.get()->setText(QKeySequence(keybindMap.at(keybindToModify).keybind).toString());
				uiGameplayKeymapGroup.get()->setTitle(uiGameplayKeymapGroupTitleDefault);
				keybindToModify = KeybindModifiable::NONE;
				gameState = GameState::CREATING;
			}
		}
	}
}

bool CreationScreen::fileSaveInterruptPromptPassed()
{
	if (!levelModified)
		return true;

	QMessageBox qMsg(this->parentWidget());
	qMsg.setStyleSheet(styleMap.at("uiMessageBoxStyle"));
	qMsg.setWindowTitle(this->parentWidget()->windowTitle());
	qMsg.setText("The level has been modified.");
	qMsg.setInformativeText("Do you want to save your changes?");
	qMsg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	qMsg.setDefaultButton(QMessageBox::Save);
	qMsg.setIconPixmap(imgMap.at("imgSavePromptWarning"));
	qMsg.setFont(uiGameplayFontTextBox);
	qMsg.button(QMessageBox::Save)->setFont(uiGameplayFontTextBox);
	qMsg.button(QMessageBox::Discard)->setFont(uiGameplayFontTextBox);
	qMsg.button(QMessageBox::Cancel)->setFont(uiGameplayFontTextBox);

	int ret = qMsg.exec();

	switch (ret)
	{
	case QMessageBox::Save:
		return fileSaveOperationType();
	case QMessageBox::Discard:
		setLevelModified(false);
		return true;
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}

	return true;
}

void CreationScreen::prefSave()
{
	QFile fileWrite(appExecutablePath + "/config.txt");
	if (fileWrite.open(QIODevice::WriteOnly))
	{
		QTextStream qStream(&fileWrite);

		qStream << "CREATOR:\r\n"
			"creatorName=" + uiGameplayStatsInputCreatorName.get()->text() + "\r\n"
			"\r\n"
			;

		qStream << "KEYBINDS: \r\n";
		for (const auto& k : keybindMap)
		{
			// We remove any spaces from the identifier for stricter specificity of searching,
			// e.g. "Open Menu" becomes "OpenMenu", then we search for "OpenMenu=" when loading.
			QString identifier = k.second.labelText;
			qStream <<
				identifier.remove(QChar(' '), Qt::CaseSensitive) +
				"=" +
				QKeySequence(k.second.keybind).toString() +
				"\r\n"
				;
		}
		fileWrite.close();
	}
}

// private:

void CreationScreen::prefLoad()
{
	QFile fileRead(appExecutablePath + "/config.txt");
	if (fileRead.open(QIODevice::ReadOnly))
	{
		QTextStream qStream(&fileRead);
		while (!qStream.atEnd())
		{
			QString line = qStream.readLine();
			if (line.contains("creatorName="))
			{
				QString newName = extractSubstringInbetweenQt("=", "", line);
				if (!newName.isEmpty())
					creatorNameDefault = newName;
				continue;
			}

			for (auto& k : keybindMap)
			{
				QString identifier = k.second.labelText;
				if (line.contains(identifier.remove(QChar(' '), Qt::CaseSensitive) + "="))
				{
					QString newKey = extractSubstringInbetweenQt("=", "", line);
					if (!newKey.isEmpty())
						k.second.keybind = Qt::Key(QKeySequence::fromString(newKey)[0]);
					continue;
				}
			}
		}
		fileRead.close();
	}
}

bool CreationScreen::fileSaveOperationType()
{
	if (fileCurrent.isEmpty())
	{
		return fileSaveAs();
	}
	else
	{
		return fileSave(fileCurrent);
	}
}

bool CreationScreen::fileSaveAs()
{
	// We remove spaces so that filename has no spaces in it.
	// Need to note this to user in documentation concerning unique hashes.
	creatorName = uiGameplayStatsInputCreatorName->text();
	creatorName.remove(QChar(' '), Qt::CaseSensitive);
	levelName = uiGameplayStatsInputLevelName->text();
	levelName.remove(QChar(' '), Qt::CaseSensitive);

	// For ID: Construct unique hash from combination of creator name and level name.
	// This is to ensure that each level created has a unique ID.
	// We use a long FNV-1 style hash algorithm, which will give us a low risk of collisions and
	// a consistent ID from the given string each time, so long as creator and level name stay the same.
	// This means the level can be saved, resaved, loaded, etc., without the ID getting changed around.
	if (!creatorName.isEmpty() && !levelName.isEmpty())
		levelId = generateHash64Hex(creatorName.toStdString() + levelName.toStdString());

	QString proposedSaveName;
	proposedSaveName += fileDirLastSaved + "/";
	if (levelId.isEmpty())
		proposedSaveName += QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
	else
		proposedSaveName += creatorName + "_" + levelId;
	QFileDialog dialog(this, tr("Save As"), proposedSaveName, tr("Moxybox Files (*.MoxyLvl)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString fpath = dialog.selectedFiles().first();
		if (fileSave(fpath))
			return true;
	}
	return false;
}

bool CreationScreen::fileSave(const QString &fpath)
{
	QFile fileWrite(fpath);
	if (fileWrite.open(QIODevice::WriteOnly))
	{
		QTextStream qStream(&fileWrite);

		qStream <<
			"::"
			"Id=" + levelId +
			"::" +
			"CreatorName=" + creatorName +
			"::" +
			"LevelName=" + levelName +
			"::" +
			"LevelDifficulty=" + uiGameplayStatsInputLevelDifficulty.get()->text() +
			"::" +
			"TurnsRemaining=" + uiGameplayStatsInputLevelTurns.get()->text() +
			"::" +
			"\r\n";

		// Gates and keys are saved on the same line for level files.
		// We do this so we can validate that the number of each is the same when loading into
		// the main game.
		qStream << "::Gate=";
		for (const auto& gate : gatesMap)
		{
			qStream <<
				"(" +
				QString::number(gate.second.item.get()->x()) +
				"," +
				QString::number(gate.second.item.get()->y()) +
				"," +
				tokenImmobile::stateToString(gate.second.state) +
				")";
		}
		qStream << "::Key=";
		for (const auto& key : keysMap)
		{
			qStream <<
				"(" +
				QString::number(key.second.item.get()->x()) +
				"," +
				QString::number(key.second.item.get()->y()) +
				"," +
				tokenImmobile::stateToString(key.second.state) +
				")";
		}
		qStream << "::\r\n";

		qStream << "::Player=";
		for (const auto& player : playersMap)
		{
			qStream <<
				QString::number(player.second.item.get()->x()) +
				"," +
				QString::number(player.second.item.get()->y());
		}
		qStream << "::\r\n";

		qStream << "::Pusher=";
		for (auto& pusher : pushersMap)
		{
			adjustPatrollerPathToWithinGridBound(pusher.second);

			qStream <<
				"(" +
				QString::number(pusher.second.item.get()->x()) +
				"," +
				QString::number(pusher.second.item.get()->y()) +
				"," +
				tokenPatroller::typeToString(pusher.second.type) +
				"," +
				tokenPatroller::facingToString(pusher.second.facing) +
				"," +
				tokenPatroller::patrolDirToString(pusher.second.patrolDir) +
				"," +
				QString::number(pusher.second.patrolBoundUp) +
				"," +
				QString::number(pusher.second.patrolBoundDown) +
				"," +
				QString::number(pusher.second.patrolBoundLeft) +
				"," +
				QString::number(pusher.second.patrolBoundRight) +
				")";
		}
		qStream << "::\r\n";

		qStream << "::Sucker=";
		for (auto& sucker : suckersMap)
		{
			adjustPatrollerPathToWithinGridBound(sucker.second);

			qStream <<
				"(" +
				QString::number(sucker.second.item.get()->x()) +
				"," +
				QString::number(sucker.second.item.get()->y()) +
				"," +
				tokenPatroller::typeToString(sucker.second.type) +
				"," +
				tokenPatroller::facingToString(sucker.second.facing) +
				"," +
				tokenPatroller::patrolDirToString(sucker.second.patrolDir) +
				"," +
				QString::number(sucker.second.patrolBoundUp) +
				"," +
				QString::number(sucker.second.patrolBoundDown) +
				"," +
				QString::number(sucker.second.patrolBoundLeft) +
				"," +
				QString::number(sucker.second.patrolBoundRight) +
				")";
		}
		qStream << "::\r\n";

		qStream << "::Util=";
		for (auto& util : utilsMap)
		{
			qStream <<
				"(" +
				QString::number(util.second.item.get()->x()) +
				"," +
				QString::number(util.second.item.get()->y()) +
				"," +
				tokenUtil::typeToString(util.second.type) +
				"," +
				tokenUtil::stateToString(util.second.state) +
				")";
		}
		qStream << "::\r\n";

		qStream << "::Block=";
		for (auto& block : blocksMap)
		{
			qStream <<
				"(" +
				QString::number(block.second.item.get()->x()) +
				"," +
				QString::number(block.second.item.get()->y()) +
				"," +
				tokenImmobile::stateToString(block.second.state) +
				")";
		}
		qStream << "::\r\n";

		qStream << "::Hazard=";
		for (auto& hazard : hazardsMap)
		{
			qStream <<
				"(" +
				QString::number(hazard.second.item.get()->x()) +
				"," +
				QString::number(hazard.second.item.get()->y()) +
				"," +
				tokenImmobile::stateToString(hazard.second.state) +
				")";
		}
		qStream << "::";

		fileWrite.close();
		fileDirLastSaved = QFileInfo(fpath).path();
		fileCurrent = fpath;
		setLevelModified(false);
		updateWindowTitle(QFileInfo(fpath).baseName());
		uiGameplayMessagesTextBox.get()->setText(uiGameplayMessagesFileSaved);
		uiMenuResumeCreating();
		return true;
	}
	return false;
}

void CreationScreen::fileNew()
{
	if (gameState == GameState::PAUSED)
	{
		if (fileSaveInterruptPromptPassed())
		{
			// UI inputs all need to be set to default values, with the exception of keybinds, 
			// which are considered to be independent from level creation,
			// since they only impact the program itself, not created/saved levels.
			uiGameplaySetToDefaults();

			// Note: We remove the items from the scene before clearing the maps,
			// to ensure there's no weirdness with parenting and destruction of pointers.
			for (const auto& obj : playersMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : pushersMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : suckersMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : blocksMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : keysMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : gatesMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : hazardsMap)
				scene.get()->removeItem(obj.second.item.get());

			for (const auto& obj : utilsMap)
				scene.get()->removeItem(obj.second.item.get());

			playersMap.clear();
			pushersMap.clear();
			suckersMap.clear();
			blocksMap.clear();
			keysMap.clear();
			gatesMap.clear();
			hazardsMap.clear();
			utilsMap.clear();

			hidePatrollerHoverDisplay();

			setLevelModified(false);
			fileCurrent = "";
			updateWindowTitle(winTitleUntitled);
			uiMenuResumeCreating();
		}
	}
}

void CreationScreen::fileLoad()
{
	if (gameState == GameState::PAUSED)
	{
		// We check with save prompt before loading if level has been modified since last save,
		// to ensure user doesn't accidentally load over work in progress they wanted to keep.
		if (!fileSaveInterruptPromptPassed())
			return;

		// reverse the saving process with magical powers of deduction
		QString filename = QFileDialog::getOpenFileName(this, tr("Open"), fileDirLastOpened, tr("Moxybox Files (*.MoxyLvl)"));
		if (!filename.isEmpty())
		{
			fileDirLastOpened = QFileInfo(filename).path();
			QFile fileRead(filename);
			if (fileRead.open(QIODevice::ReadOnly))
			{
				// Now that we've got a confirmed file that is opened, we want to set things to default before loading,
				// so that loaded in elements don't overlap or conflict with existing ones.
				fileNew();

				QTextStream qStream(&fileRead);
				while (!qStream.atEnd())
				{
					QString line = qStream.readLine();
					if (line.contains("::Id="))
					{
						levelId = extractSubstringInbetweenQt("::Id=", "::", line);
						creatorName = extractSubstringInbetweenQt("::CreatorName=", "::", line);
						levelName = extractSubstringInbetweenQt("::LevelName=", "::", line);
						levelDifficulty = extractSubstringInbetweenQt("::LevelDifficulty=", "::", line).toInt();
						levelTurns = extractSubstringInbetweenQt("::TurnsRemaining=", "::", line).toInt();

						uiGameplayUpdateStatInputCreatorName();
						uiGameplayUpdateStatInputLevelName();
						uiGameplayUpdateStatInputLevelTurns();
						uiGameplayUpdateStatInputLevelDifficulty();
					}
					else if (line.contains("::Player="))
					{
						QStringList components = extractSubstringInbetweenQt("::Player=", "::", line).split(",", QString::SkipEmptyParts);
						QString componentsKey = components[0] + components[1];
						playersMap.insert
						(
							std::pair<QString, tokenPlayer>
							(
								componentsKey,
								tokenPlayer
								{
									components[0].toDouble(),
									components[1].toDouble()
								}
							)
						);
						scene.get()->addItem(playersMap.at(componentsKey).item.get());
						playersMap.at(componentsKey).item.get()->setPixmap(imgMap.at("imgPlayer"));
						playersMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());

					}
					else if (line.contains("::Gate=") && line.contains("::Key="))
					{
						{
							QString dataLine = extractSubstringInbetweenQt("::Gate=", "::", line);
							QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

							for (int i = 0; i < dataList.length(); i++)
							{
								QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
								QString componentsKey = components[0] + components[1];
								gatesMap.insert
								(
									std::pair<QString, tokenImmobile>
									(
										componentsKey,
										tokenImmobile
										{
											components[0].toDouble(),
											components[1].toDouble(),
											tokenImmobile::Type::GATE
										}
									)
								);
								scene.get()->addItem(gatesMap.at(componentsKey).item.get());
								gatesMap.at(componentsKey).item.get()->setPixmap(imgMap.at("imgImmobileGate"));
								gatesMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
							}
						}

						{
							QString dataLine = extractSubstringInbetweenQt("::Key=", "::", line);
							QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

							for (int i = 0; i < dataList.length(); i++)
							{
								QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
								QString componentsKey = components[0] + components[1];
								keysMap.insert
								(
									std::pair<QString, tokenImmobile>
									(
										componentsKey,
										tokenImmobile
										{
											components[0].toDouble(),
											components[1].toDouble(),
											tokenImmobile::Type::KEY
										}
									)
								);
								scene.get()->addItem(keysMap.at(componentsKey).item.get());
								keysMap.at(componentsKey).item.get()->setPixmap(imgMap.at("imgImmobileKey"));
								keysMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
							}
						}
					}
					else if (line.contains("::Pusher="))
					{
						QString dataLine = extractSubstringInbetweenQt("::Pusher=", "::", line);
						QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

						for (int i = 0; i < dataList.length(); i++)
						{
							QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
							QString componentsKey = components[0] + components[1];
							pushersMap.insert
							(
								std::pair<QString, tokenPatroller>
								(
									componentsKey,
									tokenPatroller
									{
										components[0].toDouble(),
										components[1].toDouble(),
										tokenPatroller::typeToEnum(components[2]),
										tokenPatroller::facingToEnum(components[3]),
										tokenPatroller::patrolDirToEnum(components[4]),
										components[5].toInt(),
										components[6].toInt(),
										components[7].toInt(),
										components[8].toInt()
									}
								)
							);
							scene.get()->addItem(pushersMap.at(componentsKey).item.get());
							pushersMap.at(componentsKey).item.get()->setPixmap
							(
								facingToImg(pushersMap.at(componentsKey).facing, pushersMap.at(componentsKey).type)
							);
							pushersMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
						}
					}
					else if (line.contains("::Sucker="))
					{
						QString dataLine = extractSubstringInbetweenQt("::Sucker=", "::", line);
						QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

						for (int i = 0; i < dataList.length(); i++)
						{
							QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
							QString componentsKey = components[0] + components[1];
							suckersMap.insert
							(
								std::pair<QString, tokenPatroller>
								(
									componentsKey,
									tokenPatroller
									{
										components[0].toDouble(),
										components[1].toDouble(),
										tokenPatroller::typeToEnum(components[2]),
										tokenPatroller::facingToEnum(components[3]),
										tokenPatroller::patrolDirToEnum(components[4]),
										components[5].toInt(),
										components[6].toInt(),
										components[7].toInt(),
										components[8].toInt()
									}
								)
							);
							scene.get()->addItem(suckersMap.at(componentsKey).item.get());
							suckersMap.at(componentsKey).item.get()->setPixmap
							(
								facingToImg(suckersMap.at(componentsKey).facing, suckersMap.at(componentsKey).type)
							);
							suckersMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
						}
					}
					else if (line.contains("::Util="))
					{
						QString dataLine = extractSubstringInbetweenQt("::Util=", "::", line);
						QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

						for (int i = 0; i < dataList.length(); i++)
						{
							QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
							QString componentsKey = components[0] + components[1];
							utilsMap.insert
							(
								std::pair<QString, tokenUtil>
								(
									componentsKey,
									tokenUtil
									{
										components[0].toDouble(),
										components[1].toDouble(),
										tokenUtil::typeToEnum(components[2]),
										tokenUtil::stateToEnum(components[3])
									}
								)
							);
							scene.get()->addItem(utilsMap.at(componentsKey).item.get());
							utilsMap.at(componentsKey).item.get()->setPixmap
							(
								stateToImg(utilsMap.at(componentsKey).state, utilsMap.at(componentsKey).type)
							);
							utilsMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
						}
					}
					else if (line.contains("::Block="))
					{
						QString dataLine = extractSubstringInbetweenQt("::Block=", "::", line);
						QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

						for (int i = 0; i < dataList.length(); i++)
						{
							QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
							QString componentsKey = components[0] + components[1];
							blocksMap.insert
							(
								std::pair<QString, tokenImmobile>
								(
									componentsKey,
									tokenImmobile
									{
										components[0].toDouble(),
										components[1].toDouble(),
										tokenImmobile::typeToEnum(components[2])
									}
								)
							);
							scene.get()->addItem(blocksMap.at(componentsKey).item.get());
							blocksMap.at(componentsKey).item.get()->setPixmap(imgMap.at("imgImmobileBlock"));
							blocksMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
						}
					}
					else if (line.contains("::Hazard="))
					{
						QString dataLine = extractSubstringInbetweenQt("::Hazard=", "::", line);
						QStringList dataList = extractSubstringInbetweenQtLoopList("(", ")", dataLine);

						for (int i = 0; i < dataList.length(); i++)
						{
							QStringList components = dataList[i].split(",", QString::SkipEmptyParts);
							QString componentsKey = components[0] + components[1];
							hazardsMap.insert
							(
								std::pair<QString, tokenImmobile>
								(
									componentsKey,
									tokenImmobile
									{
										components[0].toDouble(),
										components[1].toDouble(),
										tokenImmobile::typeToEnum(components[2])
									}
								)
							);
							scene.get()->addItem(hazardsMap.at(componentsKey).item.get());
							hazardsMap.at(componentsKey).item.get()->setPixmap(imgMap.at("imgImmobileHazard"));
							hazardsMap.at(componentsKey).item.get()->setPos(components[0].toDouble(), components[1].toDouble());
						}
					}
				}
				fileRead.close();
				setLevelModified(false);
				fileCurrent = filename;
				updateWindowTitle(QFileInfo(filename).baseName());
				uiMenuResumeCreating();
			}
		}
	}
}

void CreationScreen::unpauseProgram()
{
	if (gameState == GameState::PAUSED)
	{
		uiMenuResumeCreating();
	}
}

void CreationScreen::exitProgram()
{
	if (gameState == GameState::PAUSED)
	{
		this->parentWidget()->parentWidget()->close();
	}
}

void CreationScreen::setLevelModified(const bool toggle)
{
	levelModified = toggle;
}

void CreationScreen::updateFadedTokenPosition()
{
	fadedToken.get()->setPos
	(
		highlightToken.get()->x() + (gridPieceSize / 4) - highlightOffsetX,
		highlightToken.get()->y() + (gridPieceSize / 4) - highlightOffsetY
	);
	qDebug() << "fadedTokenX: " << fadedToken.get()->x();
	qDebug() << "fadedTokenY: " << fadedToken.get()->y();
}

void CreationScreen::showPatrollerHoverDisplay(const std::map<QString, tokenPatroller>::iterator& it)
{
	// Because the number of hover items can differ for a given patroller, we set them all
	// to invisible image before updating, to ensure we don't have lingering images on the scene.
	hidePatrollerHoverDisplay();

	patrollerHovering = true;

	switch (it->second.facing)
	{
	case tokenPatroller::Facing::LEFT:
		for (int i = 0; i < it->second.patrolBoundLeft; i++)
		{
			if (fadedToken.get()->x() - (gridPieceSize * (i + 1)) > gridBoundLeft)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x() - (gridPieceSize * (i + 1)),
					fadedToken.get()->y()
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(it->second.facing, it->second.type)
				);
			}
		}
		break;
	case tokenPatroller::Facing::RIGHT:
		for (int i = 0; i < it->second.patrolBoundRight; i++)
		{
			if (fadedToken.get()->x() + (gridPieceSize * (i + 1)) < gridBoundRight)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x() + (gridPieceSize * (i + 1)),
					fadedToken.get()->y()
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(it->second.facing, it->second.type)
				);
			}
		}
		break;
	case tokenPatroller::Facing::UP:
		for (int i = 0; i < it->second.patrolBoundUp; i++)
		{
			if (fadedToken.get()->y() - (gridPieceSize * (i + 1)) > gridBoundUp)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x(),
					fadedToken.get()->y() - (gridPieceSize * (i + 1))
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(it->second.facing, it->second.type)
				);
			}
		}
		break;
	case tokenPatroller::Facing::DOWN:
		for (int i = 0; i < it->second.patrolBoundDown; i++)
		{
			if (fadedToken.get()->y() + (gridPieceSize * (i + 1)) < gridBoundDown)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x(),
					fadedToken.get()->y() + (gridPieceSize * (i + 1))
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(it->second.facing, it->second.type)
				);
			}
		}
		break;
	}
}

void CreationScreen::showPatrollerHoverDisplay(const std::map<QString, tokenPatroller> &map, const QString &key)
{
	// Because the number of hover items can differ for a given patroller, we set them all
	// to invisible image before updating, to ensure we don't have lingering images on the scene.
	hidePatrollerHoverDisplay();

	patrollerHovering = true;

	switch (map.at(key).facing)
	{
	case tokenPatroller::Facing::LEFT:
		for (int i = 0; i < map.at(key).patrolBoundLeft; i++)
		{
			if (fadedToken.get()->x() - (gridPieceSize * (i + 1)) > gridBoundLeft)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x() - (gridPieceSize * (i + 1)),
					fadedToken.get()->y()
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(map.at(key).facing, map.at(key).type)
				);
			}
		}
		break;
	case tokenPatroller::Facing::RIGHT:
		for (int i = 0; i < map.at(key).patrolBoundRight; i++)
		{
			if (fadedToken.get()->x() + (gridPieceSize * (i + 1)) < gridBoundRight)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x() + (gridPieceSize * (i + 1)),
					fadedToken.get()->y()
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(map.at(key).facing, map.at(key).type)
				);
			}
		}
		break;
	case tokenPatroller::Facing::UP:
		for (int i = 0; i < map.at(key).patrolBoundUp; i++)
		{
			if (fadedToken.get()->y() - (gridPieceSize * (i + 1)) > gridBoundUp)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x(),
					fadedToken.get()->y() - (gridPieceSize * (i + 1))
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(map.at(key).facing, map.at(key).type)
				);
			}
		}
		break;
	case tokenPatroller::Facing::DOWN:
		for (int i = 0; i < map.at(key).patrolBoundDown; i++)
		{
			if (fadedToken.get()->y() + (gridPieceSize * (i + 1)) < gridBoundDown)
			{
				patrollerHoverItems[i].get()->setPos
				(
					fadedToken.get()->x(),
					fadedToken.get()->y() + (gridPieceSize * (i + 1))
				);
				patrollerHoverItems[i].get()->setPixmap
				(
					facingToImgRange(map.at(key).facing, map.at(key).type)
				);
			}
		}
		break;
	}
}

void CreationScreen::hidePatrollerHoverDisplay()
{
	patrollerHovering = false;
	for (const auto& item : patrollerHoverItems)
	{
		item.get()->setPixmap(imgInvisible);
	}
}

void CreationScreen::rotatePatrollerFacingClockwise(const std::map<QString, tokenPatroller>::iterator& it)
{
	switch (it->second.facing)
	{
	case tokenPatroller::Facing::UP:
		it->second.facing = tokenPatroller::Facing::RIGHT;
		break;
	case tokenPatroller::Facing::RIGHT:
		it->second.facing = tokenPatroller::Facing::DOWN;
		break;
	case tokenPatroller::Facing::DOWN:
		it->second.facing = tokenPatroller::Facing::LEFT;
		break;
	case tokenPatroller::Facing::LEFT:
		it->second.facing = tokenPatroller::Facing::UP;
		break;
	}

	// Patrol direction needs to be changed to match updated facing, so they'll match when saved/loaded in main game.
	switch (it->second.facing)
	{
	case tokenPatroller::Facing::UP:
	case tokenPatroller::Facing::DOWN:
		it->second.patrolDir = tokenPatroller::PatrolDir::VERTICAL;
		break;
	case tokenPatroller::Facing::LEFT:
	case tokenPatroller::Facing::RIGHT:
		it->second.patrolDir = tokenPatroller::PatrolDir::HORIZONTAL;
		break;
	}
}

void CreationScreen::uiGameplaySetToDefaults()
{
	levelId = levelIdDefault;
	creatorName = creatorNameDefault;
	levelName = levelNameDefault;
	levelTurns = levelTurnsDefault;
	levelDifficulty = levelDifficultyDefault;
	uiGameplayUpdateStatInputCreatorName();
	uiGameplayUpdateStatInputLevelName();
	uiGameplayUpdateStatInputLevelTurns();
	uiGameplayUpdateStatInputLevelDifficulty();
	uiGameplayMessagesTextBox.get()->setText("");
}

void CreationScreen::uiGameplayUpdateStatInputCreatorName()
{
	uiGameplayStatsInputCreatorName->setText(creatorName);
}

void CreationScreen::uiGameplayUpdateStatInputLevelName()
{
	uiGameplayStatsInputLevelName.get()->setText(levelName);
}

void CreationScreen::uiGameplayUpdateStatInputLevelTurns()
{
	uiGameplayStatsInputLevelTurns.get()->setValue(levelTurns);
}

void CreationScreen::uiGameplayUpdateStatInputLevelDifficulty()
{
	uiGameplayStatsInputLevelDifficulty.get()->setValue(levelDifficulty);
}

void CreationScreen::uiMenuResumeCreating()
{
	gameState = GameState::CREATING;
	uiMenuGroup.get()->setVisible(false);
	uiGameplayStatsGroup.get()->setEnabled(true);
}

void CreationScreen::updateWindowTitle(const QString &filename)
{
	this->parentWidget()->parentWidget()->setWindowTitle
	(
		winTitleProgramName + " - " + winTitlePlaceholder + filename
	);
}

QPixmap CreationScreen::tokenTypeToImg(const TokenType &type)
{
	if (type == TokenType::NONE)
		return imgInvisible;
	else if (type == TokenType::PLAYER)
		return imgMap.at("imgPlayer");
	else if (type == TokenType::PUSHER)
		return imgMap.at("imgPatrollerPusherUp");
	else if (type == TokenType::SUCKER)
		return imgMap.at("imgPatrollerSuckerUp");
	else if (type == TokenType::BLOCK)
		return imgMap.at("imgImmobileBlock");
	else if (type == TokenType::KEY)
		return imgMap.at("imgImmobileKey");
	else if (type == TokenType::GATE)
		return imgMap.at("imgImmobileGate");
	else if (type == TokenType::HAZARD)
		return imgMap.at("imgImmobileHazard");
	else if (type == TokenType::UTIL_PUSH)
		return imgMap.at("imgUtilPusherInactive");
	else if (type == TokenType::UTIL_SUCK)
		return imgMap.at("imgUtilSuckerInactive");
	else
		return imgError;
}

QPixmap CreationScreen::stateToImg(const tokenImmobile::State &state, const tokenImmobile::Type &type)
{
	if (state == tokenImmobile::State::INVISIBLE || state == tokenImmobile::State::HELD)
	{
		return imgInvisible;
	}
	else if (state == tokenImmobile::State::ACTIVE)
	{
		if (type == tokenImmobile::Type::KEY)
			return imgMap.at("imgImmobileKey");
		else if (type == tokenImmobile::Type::GATE)
			return imgMap.at("imgImmobileGate");
		else if (type == tokenImmobile::Type::BLOCK)
			return imgMap.at("imgImmobileBlock");
		else if (type == tokenImmobile::Type::HAZARD)
			return imgMap.at("imgImmobileHazard");
		else
			return imgError;
	}
	else
		return imgError;
}

QPixmap CreationScreen::stateToImg(const tokenUtil::State &state, const tokenUtil::Type &type)
{
	if (type == tokenUtil::Type::PUSHER)
	{
		if (state == tokenUtil::State::INACTIVE)
			return imgMap.at("imgUtilPusherInactive");
		else if (state == tokenUtil::State::ACTIVE)
			return imgMap.at("imgUtilPusherActive");
		else if (state == tokenUtil::State::HELD)
			return imgInvisible;
		else
			return imgError;
	}
	else if (type == tokenUtil::Type::SUCKER)
	{
		if (state == tokenUtil::State::INACTIVE)
			return imgMap.at("imgUtilSuckerInactive");
		else if (state == tokenUtil::State::ACTIVE)
			return imgMap.at("imgUtilSuckerActive");
		else if (state == tokenUtil::State::HELD)
			return imgInvisible;
		else
			return imgError;
	}
	else
		return imgError;
}

QPixmap CreationScreen::facingToImg(const tokenPatroller::Facing &facing, const tokenPatroller::Type &type)
{
	if (type == tokenPatroller::Type::PUSHER)
	{
		if (facing == tokenPatroller::Facing::UP)
			return imgMap.at("imgPatrollerPusherUp");
		else if (facing == tokenPatroller::Facing::DOWN)
			return imgMap.at("imgPatrollerPusherDown");
		else if (facing == tokenPatroller::Facing::LEFT)
			return imgMap.at("imgPatrollerPusherLeft");
		else if (facing == tokenPatroller::Facing::RIGHT)
			return imgMap.at("imgPatrollerPusherRight");
		else
			return imgError;
	}
	else if (type == tokenPatroller::Type::SUCKER)
	{
		if (facing == tokenPatroller::Facing::UP)
			return imgMap.at("imgPatrollerSuckerUp");
		else if (facing == tokenPatroller::Facing::DOWN)
			return imgMap.at("imgPatrollerSuckerDown");
		else if (facing == tokenPatroller::Facing::LEFT)
			return imgMap.at("imgPatrollerSuckerLeft");
		else if (facing == tokenPatroller::Facing::RIGHT)
			return imgMap.at("imgPatrollerSuckerRight");
		else
			return imgError;
	}
	else
		return imgError;
}

QPixmap CreationScreen::facingToImgRange(const tokenPatroller::Facing &facing, const tokenPatroller::Type &type)
{
	if (type == tokenPatroller::Type::PUSHER)
	{
		if (facing == tokenPatroller::Facing::UP)
			return imgMap.at("imgPatrollerPusherUpRange");
		else if (facing == tokenPatroller::Facing::DOWN)
			return imgMap.at("imgPatrollerPusherDownRange");
		else if (facing == tokenPatroller::Facing::LEFT)
			return imgMap.at("imgPatrollerPusherLeftRange");
		else if (facing == tokenPatroller::Facing::RIGHT)
			return imgMap.at("imgPatrollerPusherRightRange");
		else
			return imgError;
	}
	else if (type == tokenPatroller::Type::SUCKER)
	{
		if (facing == tokenPatroller::Facing::UP)
			return imgMap.at("imgPatrollerSuckerUpRange");
		else if (facing == tokenPatroller::Facing::DOWN)
			return imgMap.at("imgPatrollerSuckerDownRange");
		else if (facing == tokenPatroller::Facing::LEFT)
			return imgMap.at("imgPatrollerSuckerLeftRange");
		else if (facing == tokenPatroller::Facing::RIGHT)
			return imgMap.at("imgPatrollerSuckerRightRange");
		else
			return imgError;
	}
	else
		return imgError;
}

const QFont::StyleStrategy CreationScreen::fontStrategyToEnum(const QString &str)
{
	// Default to antialias strat if string received is unexpected.
	// Moxybox uses Pixellari with NoAntialias by default, but most fonts someone
	// would want to use are probably going to want to be used with antialiasing.
	// So in the event of a mod messing up the naming to call the right font strat,
	// we default to antialiasing.
	if (str == "NoAntialias")
		return QFont::StyleStrategy::NoAntialias;
	else if (str == "NoSubpixelAntialias")
		return QFont::StyleStrategy::NoSubpixelAntialias;
	else if (str == "PreferAntialias")
		return QFont::StyleStrategy::PreferAntialias;
	else
		return QFont::StyleStrategy::PreferAntialias;
}

const QString CreationScreen::fontStrategyToString(const QFont::StyleStrategy &strat)
{
	if (strat == QFont::StyleStrategy::NoAntialias)
		return "NoAntialias";
	else if (strat == QFont::StyleStrategy::NoSubpixelAntialias)
		return "NoSubpixelAntialias";
	else if (strat == QFont::StyleStrategy::PreferAntialias)
		return "PreferAntialias";
	else
		return "PreferAntialias";
}

const QFont::Weight CreationScreen::fontWeightToEnum(const QString &str)
{
	if (str == "Normal")
		return QFont::Weight::Normal;
	else if (str == "Thin")
		return QFont::Weight::Thin;
	else if (str == "ExtraLight")
		return QFont::Weight::ExtraLight;
	else if (str == "Light")
		return QFont::Weight::Light;
	else if (str == "Medium")
		return QFont::Weight::Medium;
	else if (str == "DemiBold")
		return QFont::Weight::DemiBold;
	else if (str == "Bold")
		return QFont::Weight::Bold;
	else if (str == "ExtraBold")
		return QFont::Weight::ExtraBold;
	else if (str == "Black")
		return QFont::Weight::Black;
	else
		return QFont::Weight::Normal;
}

const QString CreationScreen::fontWeightToString(const QFont::Weight &strat)
{
	if (strat == QFont::Weight::Normal)
		return "Normal";
	else if (strat == QFont::Weight::Thin)
		return "Thin";
	else if (strat == QFont::Weight::ExtraLight)
		return "ExtraLight";
	else if (strat == QFont::Weight::Light)
		return "Light";
	else if (strat == QFont::Weight::Medium)
		return "Medium";
	else if (strat == QFont::Weight::DemiBold)
		return "DemiBold";
	else if (strat == QFont::Weight::Bold)
		return "Bold";
	else if (strat == QFont::Weight::ExtraBold)
		return "ExtraBold";
	else if (strat == QFont::Weight::Black)
		return "Black";
	else
		return "Normal";
}

QString CreationScreen::extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
			extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
			posFound = posEnd;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	return extracted;
}

void CreationScreen::adjustPatrollerPathToWithinGridBound(tokenPatroller &patroller)
{
	// We check if patrol bounds are within grid limits and reduce them to within limits.
	// This is done to ensure somebody won't end up with a level where patrollers are trying to go off screen.
	// We could check for this when adjusting patrol path in level creation, but it's probably simpler to do it
	// when saving the level. Only needs to run once per save operation that way, rather than running check every time
	// you adjust patrol path, and/or every time you place a patroller.

	while (patroller.initialX - (gridPieceSize * patroller.patrolBoundLeft) < gridBoundLeft)
	{
		patroller.patrolBoundLeft--;
	}
	while (patroller.initialX + (gridPieceSize * patroller.patrolBoundRight) > gridBoundRight)
	{
		patroller.patrolBoundRight--;
	}
	while (patroller.initialY - (gridPieceSize * patroller.patrolBoundUp) < gridBoundUp)
	{
		patroller.patrolBoundUp--;
	}
	while (patroller.initialY + (gridPieceSize * patroller.patrolBoundDown) > gridBoundDown)
	{
		patroller.patrolBoundDown--;
	}
}

QStringList CreationScreen::extractSubstringInbetweenQtLoopList(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QStringList extracted;
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
			extracted.append(strExtractFrom.mid(posBegin, posEnd - posBegin));
			posFound = posEnd;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted.append(strExtractFrom.mid(posBegin, posEnd - posBegin));
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted.append(strExtractFrom.mid(posBegin, posEnd - posBegin));
		posFound = posEnd;
	}
	return extracted;
}

QString CreationScreen::generateHash64Hex(const std::string &strToHash)
{
	const uint64_t fnvPrime64 = 1099511628211;
	const uint64_t fnvOffsetBasis64 = 14695981039346656037;

	// Generate dec hash
	uint64_t decHash = fnvOffsetBasis64;
	int i = 0;
	int lenOfStrToHash = strToHash.length();

	for (int i = 0; i < lenOfStrToHash; i++)
	{
		decHash *= fnvPrime64;
		decHash ^= (tolower(strToHash[i]));
	}

	// Modify hash into High Bit version
	unsigned long long int highBit = 0x8000000000000000;
	decHash |= highBit << 0;

	QString decHashStr = QString::number(decHash);
	QString hexHashStr;

	// Generate hex hash
	int lenOfHex = 15;

	for (int i = lenOfHex; i >= 0; i--)
	{
		unsigned int iHexDigit = ((decHash >> (4 * i)) % 16);
		if (iHexDigit < 10)
		{
			hexHashStr.append(QString::number(iHexDigit));
		}
		else
		{
			hexHashStr.append((char)('A' + (iHexDigit - 10)));
		}
	}
	return hexHashStr;
}

void CreationScreen::modLoadThemeIfExists()
{
	const QString modThemePath = appExecutablePath + "/Mods/Theme/theme.MoxyStyle";
	if (!QFile(modThemePath).exists())
		return;

	QFile fileRead(modThemePath);
	if (fileRead.open(QIODevice::ReadOnly))
	{
		QTextStream qStream(&fileRead);
		while (!qStream.atEnd())
		{
			QString line = qStream.readLine();

			// If a line has // at the beginning, we treat it as a comment and move on.
			// Doing this as a precaution in the event of a comment accidentally containing
			// a key phrase that is used to find something we want and triggering a false positive.
			// And so you can fiddle around with theme file, commenting stuff out to test,
			// without needing to delete it entirely.
			if (line[0] == '/' && line[1] == '/')
				continue;

			if (line.contains("::useTheme="))
			{
				bool useTheme = QVariant(extractSubstringInbetweenQt("::useTheme=", "::", line)).toBool();
				if (!useTheme)
				{
					fileRead.close();
					return;
				}
			}
			else if (line.contains("::standardFontFamily="))
			{
				standardFontFamily = extractSubstringInbetweenQt("::standardFontFamily=", "::", line);
			}
			else if (line.contains("::standardFontStyleStrategy="))
			{
				standardFontStyleStrategy = fontStrategyToEnum(extractSubstringInbetweenQt("::standardFontStyleStrategy=", "::", line));
			}
			else if (line.contains("::standardFontWeight="))
			{
				standardFontWeight = fontWeightToEnum(extractSubstringInbetweenQt("::standardFontWeight=", "::", line));
			}
			else if (line.contains("::uiGameplayFontGroupTitle_fontPointSize="))
			{
				uiGameplayFontGroupTitle_fontPointSize = extractSubstringInbetweenQt("::uiGameplayFontGroupTitle_fontPointSize=", "::", line).toInt();
			}
			else if (line.contains("::uiGameplayFontTextBox_fontPointSize="))
			{
				uiGameplayFontTextBox_fontPointSize = extractSubstringInbetweenQt("::uiGameplayFontTextBox_fontPointSize=", "::", line).toInt();
			}
			else if (line.contains("::uiMenuFontBtn_fontPointSize="))
			{
				uiMenuFontBtn_fontPointSize = extractSubstringInbetweenQt("::uiMenuFontBtn_fontPointSize=", "::", line).toInt();
			}

			// Due to the possibility of a stylesheet having :: as an actual part of its syntax, 
			// we use :::: as the front/back identifer instead.
			for (auto& style : styleMap)
			{
				QString identifier = "::::" + style.first + "=";
				if (line.contains(identifier))
				{
					// Images that show up in stylesheets need to have the app's executable path inserted at the beginning
					// of the path, since full path varies by user. %1 checks for insertion point, if present.
					// QString allows us to do a simple replace operation based on presence of %1.
					// If we were to have more than one kind of replacement we need to do, we can set up %2, %3, etc.
					if (line.contains("%1"))
						style.second = extractSubstringInbetweenQt(identifier, "::::", line).arg(appExecutablePath);
					else
						style.second = extractSubstringInbetweenQt(identifier, "::::", line);
					continue;
				}
			}

			for (auto& img : imgMap)
			{
				QString identifier = "::" + img.first + "=";
				if (line.contains(identifier))
				{
					img.second = QPixmap(appExecutablePath + extractSubstringInbetweenQt(identifier, "::", line));
					continue;
				}
			}
		}
		fileRead.close();
	}
}