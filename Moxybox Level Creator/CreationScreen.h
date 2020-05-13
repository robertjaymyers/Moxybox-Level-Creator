#pragma once

#include <QCoreApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDirIterator>
#include <QTextStream>
#include <QShortcut>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QTextBrowser>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <map>

class CreationScreen : public QGraphicsView
{
	Q_OBJECT

public:
	CreationScreen(QWidget *parent = nullptr);
	bool fileSaveInterruptPromptPassed();
	void prefSave();

protected:
	void wheelEvent(QWheelEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:

	std::unique_ptr<QGraphicsScene> scene = std::make_unique<QGraphicsScene>();

	const QString appExecutablePath = QCoreApplication::applicationDirPath();

	QString fileDirLastSaved = appExecutablePath + "/LevelData";
	QString fileDirLastOpened = appExecutablePath + "/LevelData";
	QString fileCurrent = "";

	const QString winTitleProgramName = "Moxybox Level Creator";
	const QString winTitlePlaceholder = "[*]";
	const QString winTitleUntitled = "Untitled";

	//QString baseStyle = "border: 0px; background-color: #000000;";
	QString standardFontFamily = "Pixellari";
	QFont::StyleStrategy standardFontStyleStrategy = QFont::NoAntialias;
	QFont::Weight standardFontWeight = QFont::Normal;
	int uiGameplayFontGroupTitle_fontPointSize = 12;
	int uiGameplayFontTextBox_fontPointSize = 12;
	int uiMenuFontBtn_fontPointSize = 14;

	enum class GameState { NONE, CREATING, PAUSED, KEYBINDING, INPUTTING };
	GameState gameState = GameState::NONE;

	bool levelModified = false;

	//---------
	// IMAGES
	//---------
	// Note that we could fiddle around with manually rotating a single QPixmap, rather than using multiple images,
	// for facing. But, this would mean if someone chooses to do an art style that is not top-down, they'll be stuck with
	// using the rotated versions of a single image for Left/Right/Up/Down. In some art styles, this means
	// it won't look right at all. The back of something might look drastically different from the front and need
	// to be drawn specifically to convey that perspective.
	std::map<QString, QPixmap> imgMap =
	{
		{ "imgTokenPlacingOutline", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPlacingOutline.png") },
		{ "imgImmobileKey", QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileKey.png") },
		{ "imgImmobileGate", QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileGate.png") },
		{ "imgImmobileBlock", QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileBlock.png") },
		{ "imgImmobileHazard", QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileHazard.png") },
		{ "imgPlayer", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPlayer.png") },
		{ "imgPatrollerPusherLeft", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherLeft.png") },
		{ "imgPatrollerPusherRight", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherRight.png") },
		{ "imgPatrollerPusherUp", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherUp.png") },
		{ "imgPatrollerPusherDown", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherDown.png") },
		{ "imgPatrollerSuckerLeft", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerLeft.png") },
		{ "imgPatrollerSuckerRight", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerRight.png") },
		{ "imgPatrollerSuckerUp", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerUp.png") },
		{ "imgPatrollerSuckerDown", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerDown.png") },
		{ "imgPatrollerPusherLeftRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherLeftRange.png") },
		{ "imgPatrollerPusherRightRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherRightRange.png") },
		{ "imgPatrollerPusherUpRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherUpRange.png") },
		{ "imgPatrollerPusherDownRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherDownRange.png") },
		{ "imgPatrollerSuckerLeftRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerLeftRange.png") },
		{ "imgPatrollerSuckerRightRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerRightRange.png") },
		{ "imgPatrollerSuckerUpRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerUpRange.png") },
		{ "imgPatrollerSuckerDownRange", QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerDownRange.png") },
		{ "imgUtilPusherInactive", QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilPusherInactive.png") },
		{ "imgUtilPusherActive", QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilPusherActive.png") },
		{ "imgUtilSuckerInactive", QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilSuckerInactive.png") },
		{ "imgUtilSuckerActive", QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilSuckerActive.png") },
		{ "imgGridCornerUpL", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerUpL.png") },
		{ "imgGridCornerUpR", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerUpR.png") },
		{ "imgGridCornerDownL", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerDownL.png") },
		{ "imgGridCornerDownR", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerDownR.png") },
		{ "imgGridInner", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureInner.png") },
		{ "imgGridEdgeUpX", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeUpX.png") },
		{ "imgGridEdgeDownX", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeDownX.png") },
		{ "imgGridEdgeLeftY", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeLeftY.png") },
		{ "imgGridEdgeRightY", QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeRightY.png") },
		{ "imgSavePromptWarning", QPixmap(":/MoxyboxLevelCreator/Resources/msg-box-warning.png") }
	};

	// Unlike other images used in the program, "invisible" and "error" do not have a thematic purpose.
	// Meaning, there is no expectation of, or point to, them being changed as part of a mod theme.
	// So we make them const, unlike the rest of the images.
	const QPixmap imgInvisible = QPixmap(":/MoxyboxLevelCreator/Resources/invisible.png");
	const QPixmap imgError = QPixmap(":/MoxyboxLevelCreator/Resources/error.png");

	// -------------
	// STYLESHEETS
	// -------------
	std::map<QString, QString> styleMap = 
	{
		{
			"baseStyle",
			"border: 0px; background-color: #000000;"
		},
		{
			"uiGameplayGroupBoxStyle",
			"QGroupBox { border-width: 1px;  border-style: solid;  border-color: #8E7320; background-color: #674003; }"
			"QGroupBox::title { border-width: 1px;  border-top-width: 0px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405;  subcontrol-origin: margin;  left: 5px; }"
			"QGroupBox::indicator { border-width: 0px;  background-color: #8D9E45;  color: #191405; }"
			"QGroupBox::indicator:checked { image: url(:/MoxyboxLevelCreator/Resources/uiGameplayCheckBoxChecked.png); }"
			"QGroupBox::indicator:unchecked { image: url(:/MoxyboxLevelCreator/Resources/uiGameplayCheckBoxUnchecked.png); }"
		},
		{
			"uiGameplayGroupBoxInnerStyle",
			"QGroupBox { border-width: 0px;  background-color: #674003; }"
		},
		{
			"uiGameplayTextBoxStyle",
			"QTextEdit { border: 0px;  background-color: #674003;  color: #C4BB81; }"
		},
		{
			"uiGameplayLabelStyle",
			"QLabel { border: 0px;  background-color: #674003;  color: #C4BB81; }"
			"QLabel:disabled { border: 0px;  background-color: #674003;  color: #7E5F23; }"
		},
		{
			"uiGameplayLineEditStyle",
			"QLineEdit { border-width: 1px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405; }"
			"QLineEdit:disabled { border-width: 1px;  border-style: solid;  border-color: #533403;  background-color: #705713;  color: #533403; }"
		},
		{
			"uiGameplayNumEditStyle",
			"QSpinBox { border-width: 1px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405; }"
			"QSpinBox:disabled { border-width: 1px;  border-style: solid;  border-color: #533403;  background-color: #705713;  color: #533403; }"
		},
		{
			"uiGameplayKeymapBtnStyle",
			"QPushButton { border-width: 1px;  border-style: solid;  border-color: #674003;  background-color: #674003;  color: #C4BB81; }"
		},
		{
			"uiGameplayKeymapModifBtnStyle",
			"QPushButton { border-width: 1px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405; }"
		},
		{
			"uiMenuGroupBoxStyle",
			"QGroupBox { background-image: url(:/MoxyboxLevelCreator/Resources/uiMenuBtsBg.png); }"
		},
		{
			"uiMenuBtnStyle",
			"QPushButton { background-color: rgba(255, 255, 255, 0);  border: none;  color: #C4BB81;  image: url(:/MoxyboxLevelCreator/Resources/invisible.png); }"
		},
		{
			"uiMessageBoxStyle",
			"QMessageBox"
			"{"
				"border-width: 1px;"
				"border-style: solid;"
				"border-color: #8E7320;"
				"background-color: #674003;"
				"color: #C4BB81;"
			"}"
			"QMessageBox QLabel"
			"{"
				"border-width: 1px;"
				"border-style: solid;"
				"border-color: #674003;"
				"background-color: #674003;"
				"color: #C4BB81;"
				"padding: 3px;"
			"}"
			"QMessageBox QPushButton"
			"{"
				"border-width: 2px;"
				"border-style: solid;"
				"border-color: #191405;"
				"background-color: #8D9E45;"
				"color: #191405;"
				"padding: 2px 12px 2px 12px;"
			"}"
		}
	};

	// -------
	// TOKEN
	// -------
	// Currently there is only one possible player, so the player object accessed
	// should always be the first (and only) one in the players object vector (index 0).
	// You may be wondering: Why store the object in a vector at all if there is only one.
	// Reason: This is done to simplify the data storing/loading process for levels and keep it the same for all types of data.
	// It also leaves less obstacles to changing players to have more than one in the future, if desired.
	int pIndex = 0;

	//const QPixmap imgTokenPlacingOutline = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPlacingOutline.png");

	//QPixmap imgImmobileKey = QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileKey.png");
	//QPixmap imgImmobileGate = QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileGate.png");
	//QPixmap imgImmobileBlock = QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileBlock.png");
	//QPixmap imgImmobileHazard = QPixmap(":/MoxyboxLevelCreator/Resources/tokenImmobileHazard.png");

	//QPixmap imgPlayer = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPlayer.png");

	// Note that we could fiddle around with manually rotating a single QPixmap, rather than using multiple images.
	// But, this would mean if someone chooses to do an art style that is not top-down, they'll be stuck with
	// using the rotated versions of a single image for Left/Right/Up/Down. In some art styles, this means
	// it won't look right at all. The back of something might look drastically different from the front and need
	// to be drawn specifically to convey that perspective.
	//QPixmap imgPatrollerPusherLeft = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherLeft.png");
	//QPixmap imgPatrollerPusherRight = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherRight.png");
	//QPixmap imgPatrollerPusherUp = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherUp.png");
	//QPixmap imgPatrollerPusherDown = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherDown.png");
	//QPixmap imgPatrollerSuckerLeft = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerLeft.png");
	//QPixmap imgPatrollerSuckerRight = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerRight.png");
	//QPixmap imgPatrollerSuckerUp = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerUp.png");
	//QPixmap imgPatrollerSuckerDown = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerDown.png");

	//QPixmap imgPatrollerPusherLeftRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherLeftRange.png");
	//QPixmap imgPatrollerPusherRightRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherRightRange.png");
	//QPixmap imgPatrollerPusherUpRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherUpRange.png");
	//QPixmap imgPatrollerPusherDownRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerPusherDownRange.png");
	//QPixmap imgPatrollerSuckerLeftRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerLeftRange.png");
	//QPixmap imgPatrollerSuckerRightRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerRightRange.png");
	//QPixmap imgPatrollerSuckerUpRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerUpRange.png");
	//QPixmap imgPatrollerSuckerDownRange = QPixmap(":/MoxyboxLevelCreator/Resources/tokenPatrollerSuckerDownRange.png");

	//QPixmap imgUtilPusherInactive = QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilPusherInactive.png");
	//QPixmap imgUtilPusherActive = QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilPusherActive.png");
	//QPixmap imgUtilSuckerInactive = QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilSuckerInactive.png");
	//QPixmap imgUtilSuckerActive = QPixmap(":/MoxyboxLevelCreator/Resources/tokenUtilSuckerActive.png");

	// Z value for mobile tokens should be higher than that of grid pieces to be atop them.
	// And higher than that of immobile tokens (ex: player would be atop utility token).
	const int tokenMobileZ = 2;
	const int tokenImmobileZ = 1;

	const int tokenSize = 20;

	struct tokenPlayer
	{
		const qreal initialX;
		const qreal initialY;
		enum class Facing { UP, DOWN, LEFT, RIGHT };
		Facing facing = Facing::UP;
		int heldKeys = 0;
		std::vector<int> heldUtilPushIndex;
		std::vector<int> heldUtilSuckIndex;
		int movementSpeed = 1; // This only checks collision for square landed on, so keep this in mind if changing it (it could break level design)
		int knockbackAmount = 2; // Be careful about what this is set to. Collision detection is likely expensive and runs for each square pushed back	
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
	};
	struct tokenPatroller
	{
		// Patrollers use initial Y/X values along with bound points to leash their patrol path.
		// Make sure bound points keep each patroller within the bounds of the grid and not colliding with anything silly.
		// Patrollers have no collision detection for screen or objects (other than player), so you must consciously set 
		// their patrol bounds to avoid silly behavior.

		// Note: Patrol direction, type, patrol bound, are const at level load, so
		// we don't need to save or load them as part of save game file.
		// (not to be confused with level data file)
		const qreal initialX;
		const qreal initialY;
		enum class Type { PUSHER, SUCKER, ERROR };
		const Type type = Type::PUSHER;
		enum class Facing { UP, DOWN, LEFT, RIGHT, ERROR };
		Facing facing = Facing::UP;
		enum class PatrolDir { VERTICAL, HORIZONTAL, ERROR };
		PatrolDir patrolDir = PatrolDir::VERTICAL;
		int patrolBoundUp = 3;
		int patrolBoundDown = 3;
		int patrolBoundLeft = 3;
		int patrolBoundRight = 3;
		int movementSpeed = 1;
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);

		static const Facing facingToEnum(const QString &str)
		{
			if (str == "UP")
				return Facing::UP;
			else if (str == "DOWN")
				return Facing::DOWN;
			else if (str == "LEFT")
				return Facing::LEFT;
			else if (str == "RIGHT")
				return Facing::RIGHT;
			else
				return Facing::ERROR;
		}

		static const QString facingToString(const Facing &facing)
		{
			if (facing == Facing::UP)
				return "UP";
			else if (facing == Facing::DOWN)
				return "DOWN";
			else if (facing == Facing::LEFT)
				return "LEFT";
			else if (facing == Facing::RIGHT)
				return "RIGHT";
			else
				return "ERROR";
		}

		static const PatrolDir patrolDirToEnum(const QString &str)
		{
			if (str == "VERTICAL")
				return PatrolDir::VERTICAL;
			else if (str == "HORIZONTAL")
				return PatrolDir::HORIZONTAL;
			else
				return PatrolDir::ERROR;
		}

		static const QString patrolDirToString(const PatrolDir &patrolDir)
		{
			if (patrolDir == PatrolDir::VERTICAL)
				return "VERTICAL";
			else if (patrolDir == PatrolDir::HORIZONTAL)
				return "HORIZONTAL";
			else
				return "ERROR";
		}

		static const Type typeToEnum(const QString &str)
		{
			if (str == "PUSHER")
				return Type::PUSHER;
			else if (str == "SUCKER")
				return Type::SUCKER;
			else
				return Type::ERROR;
		}

		static const QString typeToString(const Type &type)
		{
			if (type == Type::PUSHER)
				return "PUSHER";
			else if (type == Type::SUCKER)
				return "SUCKER";
			else
				return "ERROR";
		}
	};
	struct tokenImmobile
	{
		const qreal initialX;
		const qreal initialY;
		enum class Type { BLOCK, KEY, GATE, HAZARD, ERROR };
		Type type = Type::BLOCK;
		enum class State { ACTIVE, INVISIBLE, HELD, ERROR };
		State state = State::ACTIVE;
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);

		static const State stateToEnum(const QString &str)
		{
			if (str == "ACTIVE")
				return State::ACTIVE;
			else if (str == "INVISIBLE")
				return State::INVISIBLE;
			else if (str == "HELD")
				return State::HELD;
			else
				return State::ERROR;
		}

		static const QString stateToString(const State &state)
		{
			if (state == State::ACTIVE)
				return "ACTIVE";
			else if (state == State::INVISIBLE)
				return "INVISIBLE";
			else if (state == State::HELD)
				return "HELD";
			else
				return "ERROR";
		}

		static const Type typeToEnum(const QString &str)
		{
			if (str == "BLOCK")
				return Type::BLOCK;
			else if (str == "KEY")
				return Type::KEY;
			else if (str == "GATE")
				return Type::GATE;
			else if (str == "HAZARD")
				return Type::HAZARD;
			else
				return Type::ERROR;
		}

		static const QString typeToString(const Type &type)
		{
			if (type == Type::BLOCK)
				return "BLOCK";
			else if (type == Type::KEY)
				return "KEY";
			else if (type == Type::GATE)
				return "GATE";
			else if (type == Type::HAZARD)
				return "HAZARD";
			else
				return "ERROR";
		}
	};
	struct tokenUtil
	{
		// Note:
		// Traps don't change patrol route of patrollers.
		// Suck trap can change the axis the patroller is patrolling on, if it's an axis that isn't their patrol axis.
		// e.g. if patrolling on X axis, they can be sucked down or up to another Y row in the grid.
		// They will stay on that Y unless sucked again in another Y direction,
		// because their patrol route is leashed to X axis, not to Y axis.

		const qreal initialX;
		const qreal initialY;
		enum class Type { PUSHER, SUCKER, ERROR };
		const Type type = Type::PUSHER;
		enum class State { INACTIVE, ACTIVE, HELD, ERROR };
		State state = State::INACTIVE;
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);

		static const Type typeToEnum(const QString &str)
		{
			if (str == "PUSHER")
				return Type::PUSHER;
			else if (str == "SUCKER")
				return Type::SUCKER;
			else
				return Type::ERROR;
		}

		static const QString typeToString(const Type &type)
		{
			if (type == Type::PUSHER)
				return "PUSHER";
			else if (type == Type::SUCKER)
				return "SUCKER";
			else
				return "ERROR";
		}

		static const State stateToEnum(const QString &str)
		{
			if (str == "INACTIVE")
				return State::INACTIVE;
			else if (str == "ACTIVE")
				return State::ACTIVE;
			else if (str == "HELD")
				return State::HELD;
			else
				return State::ERROR;
		}

		static const QString stateToString(const State &state)
		{
			if (state == State::INACTIVE)
				return "INACTIVE";
			else if (state == State::ACTIVE)
				return "ACTIVE";
			else if (state == State::HELD)
				return "HELD";
			else
				return "ERROR";
		}
	};

	// ------
	// GRID
	// ------
	struct gridPieceInner // No facing orientation needed (appearance identical no matter rotation)
	{
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
	};
	struct gridPieceEdge // Facing orientation is based on up/down/left/right
	{
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
		enum class Facing { UP, DOWN, LEFT, RIGHT };
		Facing facing = Facing::UP;
	};
	struct gridPieceCorner // Facing orientation is based on upperLeft/upperRight/lowerLeft/lowerRight
	{
		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
		enum class Facing { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
		Facing facing = Facing::UP_LEFT;
	};
	std::vector<gridPieceInner> gridPiecesInner;
	std::vector<gridPieceEdge> gridPiecesEdge;
	std::vector<gridPieceCorner> gridPiecesCorner;
	const int gridPieceZ = 0; // Z value for grid pieces should be lower than that of tokens to be under them.
	const int gridRowSize = 20; // This should not be higher than screenWidth / gridPieceSizeDst
	const int gridColSize = 10; // This should not be higher than ((2/3rds of screenHeight) / gridPieceSizeDst) to have room for UI below it
	const int gridPieceSizeDefault = 40;
	int gridPieceSize = gridPieceSizeDefault;
	const int gridPieceSizeDstMultiplier = 2;
	const int gridPieceSizeSrc = 40;
	const int gridAnchorX = 0;
	const int gridAnchorY = 0;
	int gridWidth = gridRowSize * gridPieceSize;
	int gridHeight = gridColSize * gridPieceSize;
	int gridBoundUp = gridAnchorY;
	int gridBoundDown = gridHeight + gridAnchorY;
	int gridBoundLeft = gridAnchorX;
	int gridBoundRight = gridWidth + gridAnchorX;

	const int screenWidth = 800;
	const int screenHeight = 600;

	//QPixmap imgGridCornerUpL = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerUpL.png");
	//QPixmap imgGridCornerUpR = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerUpR.png");
	//QPixmap imgGridCornerDownL = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerDownL.png");
	//QPixmap imgGridCornerDownR = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureCornerDownR.png");
	//QPixmap imgGridInner = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureInner.png");
	//QPixmap imgGridEdgeUpX = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeUpX.png");
	//QPixmap imgGridEdgeDownX = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeDownX.png");
	//QPixmap imgGridEdgeLeftY = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeLeftY.png");
	//QPixmap imgGridEdgeRightY = QPixmap(":/MoxyboxLevelCreator/Resources/gridTextureEdgeRightY.png");

	// ------------
	// LEVEL DATA
	// ------------
	const QString levelIdDefault = "";
	QString creatorNameDefault = "Nobody";
	const QString levelNameDefault = "Untitled";
	const int levelTurnsDefault = 50;
	const int levelDifficultyDefault = 1;

	QString levelId = levelIdDefault; // Create this from hash combination of creator name and level name.
	QString creatorName = creatorNameDefault;
	QString levelName = levelNameDefault;
	int levelTurns = levelTurnsDefault;
	int levelDifficulty = levelDifficultyDefault;

	std::map<QString, tokenPlayer> playersMap;
	std::map<QString, tokenPatroller> pushersMap;
	std::map<QString, tokenPatroller> suckersMap;
	std::map<QString, tokenImmobile> blocksMap;
	std::map<QString, tokenImmobile> keysMap;
	std::map<QString, tokenImmobile> gatesMap;
	std::map<QString, tokenImmobile> hazardsMap;
	std::map<QString, tokenUtil> utilsMap;

	const int highlightOffsetY = 2;
	const int highlightOffsetX = 2;
	const int highlightMovementSpeed = 1;
	std::unique_ptr<QGraphicsPixmapItem> highlightToken = std::make_unique<QGraphicsPixmapItem>(nullptr);

	enum class TokenType { NONE, PLAYER, PUSHER, SUCKER, BLOCK, KEY, GATE, HAZARD, UTIL_PUSH, UTIL_SUCK };
	TokenType tokenType = TokenType::NONE;
	const int fadedMin = 0;
	const int fadedMax = 9; // Should match number of faded types (not including 'none').
	int fadedNum = fadedMin;

	std::vector<TokenType> tokenTypeList =
	{
		TokenType::NONE,
		TokenType::PLAYER,
		TokenType::PUSHER,
		TokenType::SUCKER,
		TokenType::BLOCK,
		TokenType::HAZARD,
		TokenType::KEY,
		TokenType::GATE,
		TokenType::UTIL_PUSH,
		TokenType::UTIL_SUCK,
	};

	std::map<TokenType, QString> tokenDisplayNameMap =
	{
		{ TokenType::PLAYER, "Player" },
		{ TokenType::PUSHER, "Pusher (Patroller)" },
		{ TokenType::SUCKER, "Sucker (Patroller)" },
		{ TokenType::BLOCK, "Block" },
		{ TokenType::HAZARD, "Hazard" },
		{ TokenType::KEY, "Key" },
		{ TokenType::GATE, "Gate" },
		{ TokenType::UTIL_PUSH, "Spring Trap (Utility)" },
		{ TokenType::UTIL_SUCK, "Magnet Trap (Utility)" }
	};

	bool patrollerHovering = false;
	const int patrollerHoverZValue = 5; // We make this a high value to ensure patrol path display covers other things in scene.
	std::vector<std::unique_ptr<QGraphicsPixmapItem>> patrollerHoverItems;

	std::unique_ptr<QGraphicsPixmapItem> fadedToken = std::make_unique<QGraphicsPixmapItem>(nullptr);

	// -------------
	// UI GAMEPLAY
	// -------------
	const int uiGameplayKeymapBtnSize = 20;

	QFont uiGameplayFontGroupTitle;
	QFont uiGameplayFontTextBox;

	std::unique_ptr<QGroupBox> uiGameplayGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> uiGameplayLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QGroupBox> uiGameplayStatsGroup = std::make_unique<QGroupBox>();
	std::unique_ptr<QGroupBox> uiGameplayMessagesGroup = std::make_unique<QGroupBox>();
	std::unique_ptr<QGroupBox> uiGameplayKeymapGroup = std::make_unique<QGroupBox>();

	std::unique_ptr<QGridLayout> uiGameplayStatsLayout = std::make_unique<QGridLayout>();
	std::unique_ptr<QGridLayout> uiGameplayMessagesLayout = std::make_unique<QGridLayout>();
	std::unique_ptr<QGridLayout> uiGameplayKeymapLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QGroupBox> uiGameplayStatsNameGroup = std::make_unique<QGroupBox>();
	std::unique_ptr<QGroupBox> uiGameplayStatsNumGroup = std::make_unique<QGroupBox>();

	std::unique_ptr<QVBoxLayout> uiGameplayStatsNameLayout = std::make_unique<QVBoxLayout>();
	std::unique_ptr<QGridLayout> uiGameplayStatsNumLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QLabel> uiGameplayStatsLabelCreatorName = std::make_unique<QLabel>();
	std::unique_ptr<QLabel> uiGameplayStatsLabelLevelName = std::make_unique<QLabel>();

	std::unique_ptr<QLineEdit> uiGameplayStatsInputCreatorName = std::make_unique<QLineEdit>();
	std::unique_ptr<QLineEdit> uiGameplayStatsInputLevelName = std::make_unique<QLineEdit>();

	const int spinBoxMaximum = 1000000;

	std::unique_ptr<QLabel> uiGameplayStatsLabelLevelTurns = std::make_unique<QLabel>();
	std::unique_ptr<QLabel> uiGameplayStatsLabelLevelDifficulty = std::make_unique<QLabel>();

	std::unique_ptr<QSpinBox> uiGameplayStatsInputLevelTurns = std::make_unique<QSpinBox>();
	std::unique_ptr<QSpinBox> uiGameplayStatsInputLevelDifficulty = std::make_unique<QSpinBox>();

	std::unique_ptr<QTextBrowser> uiGameplayObjectivesTextBox = std::make_unique<QTextBrowser>();
	std::unique_ptr<QTextBrowser> uiGameplayMessagesTextBox = std::make_unique<QTextBrowser>();

	// We set up an enum ID for each modifiable keybind, so that when the UI is clicked
	// to modify a key, we know which one to apply the modification to after key input.
	enum class KeybindModifiable
	{
		NONE,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		OPEN_MENU,
		PLACE_TOKEN,
		DELETE_TOKEN,
		ROTATE_PATROLLER,
		PATROL_RANGE_INC,
		PATROL_RANGE_DEC
	};
	KeybindModifiable keybindToModify = KeybindModifiable::NONE;

	struct keybindComponent
	{
		const QString labelText;
		Qt::Key keybind;
		const int uiButtonGridLayoutRow;
		const int uiButtonGridLayoutCol;
		const int uiLabelGridLayoutRow;
		const int uiLabelGridLayoutCol;
		Qt::Alignment gridLayoutAlign;
		std::unique_ptr<QPushButton> uiButton = std::make_unique<QPushButton>();
		std::unique_ptr<QLabel> uiLabel = std::make_unique<QLabel>();
	};
	std::map<KeybindModifiable, keybindComponent> keybindMap;

	const QString uiGameplayKeymapGroupTitleDefault = "Keybinds";
	const QString uiGameplayKeymapGroupTitleKeybinding = "Hit Key to Set";

	/*QString uiGameplayGroupBoxStyle =
	{
		"QGroupBox { border-width: 1px;  border-style: solid;  border-color: #8E7320; background-color: #674003; }"
		"QGroupBox::title { border-width: 1px;  border-top-width: 0px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405;  subcontrol-origin: margin;  left: 5px; }"
		"QGroupBox::indicator { border-width: 0px;  background-color: #8D9E45;  color: #191405; }"
		"QGroupBox::indicator:checked { image: url(:/MoxyboxLevelCreator/Resources/uiGameplayCheckBoxChecked.png); }"
		"QGroupBox::indicator:unchecked { image: url(:/MoxyboxLevelCreator/Resources/uiGameplayCheckBoxUnchecked.png); }"
	};*/

	/*QString uiGameplayGroupBoxInnerStyle =
	{
		"QGroupBox { border-width: 0px;  background-color: #674003; }"
	};*/

	const int minBoxWidth = (screenWidth / 4) - 6;

	/*QString uiGameplayTextBoxStyle =
	{
		"QTextEdit { border: 0px;  background-color: #674003;  color: #C4BB81; }"
	};

	QString uiGameplayLabelStyle =
	{
		"QLabel { border: 0px;  background-color: #674003;  color: #C4BB81; }"
		"QLabel:disabled { border: 0px;  background-color: #674003;  color: #7E5F23; }"
	};

	QString uiGameplayLineEditStyle =
	{
		"QLineEdit { border-width: 1px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405; }"
		"QLineEdit:disabled { border-width: 1px;  border-style: solid;  border-color: #533403;  background-color: #705713;  color: #533403; }"
	};

	QString uiGameplayNumEditStyle =
	{
		"QSpinBox { border-width: 1px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405; }"
		"QSpinBox:disabled { border-width: 1px;  border-style: solid;  border-color: #533403;  background-color: #705713;  color: #533403; }"
	};

	QString uiGameplayKeymapBtnStyle =
	{
		"QPushButton { border-width: 1px;  border-style: solid;  border-color: #674003;  background-color: #674003;  color: #C4BB81; }"
	};

	QString uiGameplayKeymapModifBtnStyle =
	{
		"QPushButton { border-width: 1px;  border-style: solid;  border-color: #191405;  background-color: #8D9E45;  color: #191405; }"
	};*/

	const QString uiGameplayMessagesTokenPlaced = " token placed.";
	const QString uiGameplayMessagesTokenDeleted = " token deleted.";
	const QString uiGameplayMessagesSquareOccupied = "That square is already in use.";
	const QString uiGameplayMessagesPlayerAlreadyPlaced = "A player token is already placed. There can only be one.";
	const QString uiGameplayMessagesObstacle = "Need force of impact to get past obstacles.";
	const QString uiGameplayMessagesKeyObtained = "You got a key! Keys can be used to open pods.";
	const QString uiGameplayMessagesKeyNeeded = "Key needed to open pod.";
	const QString uiGameplayMessagesTrapPusherObtained = "You picked up a Spring Trap! Spring traps will knock back patrollers who hit them.";
	const QString uiGameplayMessagesTrapPusherDeployed = "Spring Trap deployed.";
	const QString uiGameplayMessagesTrapSuckerObtained = "You picked up a Magnet Trap! Magnet Traps will pull in patrollers who come near.";
	const QString uiGameplayMessagesTrapSuckerDeployed = "Magnet Trap deployed.";
	const QString uiGameplayMessagesFileSaved = "File saved.";

	// ---------
	// UI MENU
	// ---------
	QFont uiMenuFontBtn;

	const int uiMenuWidth = 214;
	const int uiMenuHeight = 425;

	const int uiMenuBtnWidth = uiMenuWidth;
	const int uiMenuBtnHeight = 50;
	const int uiMenuBtnSpacing = 25;

	std::unique_ptr<QGroupBox> uiMenuGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> uiMenuLayout = std::make_unique<QGridLayout>();
	std::unique_ptr<QPushButton> uiMenuBtnResume = std::make_unique<QPushButton>();
	std::unique_ptr<QPushButton> uiMenuBtnSave = std::make_unique<QPushButton>();
	std::unique_ptr<QPushButton> uiMenuBtnSaveAs = std::make_unique<QPushButton>();
	std::unique_ptr<QPushButton> uiMenuBtnLoad = std::make_unique<QPushButton>();
	std::unique_ptr<QPushButton> uiMenuBtnReset = std::make_unique<QPushButton>();
	std::unique_ptr<QPushButton> uiMenuBtnExit = std::make_unique<QPushButton>();

	enum class UiMenuBtnType
	{
		RESUME,
		SAVE,
		SAVE_AS,
		LOAD,
		NEW,
		EXIT
	};
	struct uiMenuBtnComponent
	{
		const QString text;
		const int gridLayoutRow;
		const int gridLayoutCol;
		Qt::Alignment gridLayoutAlign;
		std::unique_ptr<QPushButton> widget = std::make_unique<QPushButton>();
	};
	std::map<UiMenuBtnType, uiMenuBtnComponent> uiMenuBtnMap;

	/*QString uiMenuGroupBoxStyle =
	{
		"QGroupBox { background-image: url(:/MoxyboxLevelCreator/Resources/uiMenuBtsBg.png); }"
	};

	QString uiMenuBtnStyle =
	{
		"QPushButton { background-color: rgba(255, 255, 255, 0);  border: none;  color: #C4BB81;  image: url(:/MoxyboxLevelCreator/Resources/invisible.png); }"
	};*/

	//QPixmap imgSavePromptWarning = QPixmap(":/MoxyboxLevelCreator/Resources/msg-box-warning.png");

	/*QString uiMenuSavePromptWarningStyle =
	{
		"QMessageBox"
		"{"
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: #8E7320;"
			"background-color: #674003;"
			"color: #C4BB81;"
		"}"
		"QMessageBox QLabel"
		"{"
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: #674003;"
			"background-color: #674003;"
			"color: #C4BB81;"
			"padding: 3px;"
		"}"
		"QMessageBox QPushButton"
		"{"
			"border-width: 2px;"
			"border-style: solid;"
			"border-color: #191405;"
			"background-color: #8D9E45;"
			"color: #191405;"
			"padding: 2px 12px 2px 12px;"
		"}"
	};*/

	// -----------
	// FUNCTIONS
	// -----------
	void prefLoad();
	bool fileSaveOperationType();
	bool fileSaveAs();
	bool fileSave(const QString &fpath);
	void fileNew();
	void fileLoad();
	void unpauseProgram();
	void exitProgram();
	void setLevelModified(const bool toggle);
	void updateFadedTokenPosition();
	void showPatrollerHoverDisplay(const std::map<QString, tokenPatroller>::iterator& it);
	void showPatrollerHoverDisplay(const std::map<QString, tokenPatroller> &map, const QString &key);
	void hidePatrollerHoverDisplay();
	void rotatePatrollerFacingClockwise(const std::map<QString, tokenPatroller>::iterator& it);
	void uiGameplaySetToDefaults();
	void uiGameplayUpdateStatInputCreatorName();
	void uiGameplayUpdateStatInputLevelName();
	void uiGameplayUpdateStatInputLevelTurns();
	void uiGameplayUpdateStatInputLevelDifficulty();
	void uiMenuResumeCreating();
	void updateWindowTitle(const QString &filename);
	QPixmap tokenTypeToImg(const TokenType &type);
	QPixmap stateToImg(const tokenImmobile::State &state, const tokenImmobile::Type &type);
	QPixmap stateToImg(const tokenUtil::State &state, const tokenUtil::Type &type);
	QPixmap facingToImg(const tokenPatroller::Facing &facing, const tokenPatroller::Type &type);
	QPixmap facingToImgRange(const tokenPatroller::Facing &facing, const tokenPatroller::Type &type);
	const QFont::StyleStrategy fontStrategyToEnum(const QString &str);
	const QString fontStrategyToString(const QFont::StyleStrategy &strat);
	const QFont::Weight fontWeightToEnum(const QString &str);
	const QString fontWeightToString(const QFont::Weight &strat);
	void adjustPatrollerPathToWithinGridBound(tokenPatroller &patroller);
	QString extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList extractSubstringInbetweenQtLoopList(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QString generateHash64Hex(const std::string &strToHash);
	void modLoadThemeIfExists();
};