#include "main_controller.h"
#include "../global_preferences.h"
#include <apps/i18n.h>
#include <assert.h>

using namespace Poincare;
using namespace Shared;
using namespace Escher;

namespace Settings {

constexpr SettingsMessageTree s_modelAngleChildren[3] = {SettingsMessageTree(I18n::Message::Degrees), SettingsMessageTree(I18n::Message::Radian), SettingsMessageTree(I18n::Message::Gradians)};
constexpr SettingsMessageTree s_modelEditionModeChildren[2] = {SettingsMessageTree(I18n::Message::Edition2D), SettingsMessageTree(I18n::Message::EditionLinear)};
constexpr SettingsMessageTree s_modelFloatDisplayModeChildren[4] = {SettingsMessageTree(I18n::Message::Decimal), SettingsMessageTree(I18n::Message::Scientific), SettingsMessageTree(I18n::Message::Engineering), SettingsMessageTree(I18n::Message::SignificantFigures)};
constexpr SettingsMessageTree s_modelComplexFormatChildren[3] = {SettingsMessageTree(I18n::Message::Real), SettingsMessageTree(I18n::Message::Cartesian), SettingsMessageTree(I18n::Message::Polar)};
constexpr SettingsMessageTree s_modelFontChildren[2] = {SettingsMessageTree(I18n::Message::LargeFont), SettingsMessageTree(I18n::Message::SmallFont)};
constexpr SettingsMessageTree s_modelAboutChildren[3] = {SettingsMessageTree(I18n::Message::SoftwareVersion), SettingsMessageTree(I18n::Message::SerialNumber), SettingsMessageTree(I18n::Message::FccId)};

MainController::MainController(Responder * parentResponder, InputEventHandlerDelegate * inputEventHandlerDelegate) :
  ViewController(parentResponder),
  m_brightnessCell(I18n::Message::Default),
  m_popUpCell(I18n::Message::Default),
  m_selectableTableView(this),
  m_preferencesController(this),
  m_displayModeController(this, inputEventHandlerDelegate),
  m_localizationController(this, LocalizationController::Mode::Language),
  m_examModeController(this),
  m_aboutController(this)
{
  // for (int i = 0; i < k_numberOfSimpleChevronCells; i++) {
  //   m_cells[i].setMessageFont(KDFont::LargeFont);
  // }
}

View * MainController::view() {
  return &m_selectableTableView;
}

void MainController::didBecomeFirstResponder() {
  if (selectedRow() < 0) {
    selectCellAtLocation(0, 0);
  }
  Container::activeApp()->setFirstResponder(&m_selectableTableView);
}

bool MainController::handleEvent(Ion::Events::Event event) {
  GlobalPreferences * globalPreferences = GlobalPreferences::sharedGlobalPreferences();
  int rowIndex = selectedRow();

  if (hasPrompt() && rowIndex == k_indexOfPopUpCell) {
    if (event == Ion::Events::OK || event == Ion::Events::EXE) {
      globalPreferences->setShowPopUp(!globalPreferences->showPopUp());
      m_selectableTableView.reloadCellAtLocation(m_selectableTableView.selectedColumn(), m_selectableTableView.selectedRow());
      return true;
    }
    return false;
  }

  if (rowIndex == k_indexOfBrightnessCell
   && (event == Ion::Events::Left || event == Ion::Events::Right || event == Ion::Events::Minus || event == Ion::Events::Plus)) {
    int delta = Ion::Backlight::MaxBrightness/GlobalPreferences::NumberOfBrightnessStates;
    int direction = (event == Ion::Events::Right || event == Ion::Events::Plus) ? delta : -delta;
    globalPreferences->setBrightnessLevel(globalPreferences->brightnessLevel()+direction);
    m_selectableTableView.reloadCellAtLocation(m_selectableTableView.selectedColumn(), m_selectableTableView.selectedRow());
    return true;
  }

  if (event == Ion::Events::OK || event == Ion::Events::EXE || event == Ion::Events::Right) {
    if (rowIndex == k_indexOfBrightnessCell) {
      /* Nothing is supposed to happen when OK or EXE are pressed on the
       * brightness cell. The case of pressing Right has been handled above. */
      return true;
    }

    if (rowIndex == k_indexOfLanguageCell) {
      m_localizationController.setMode(LocalizationController::Mode::Language);
    } else if (rowIndex == k_indexOfCountryCell) {
      m_localizationController.setMode(LocalizationController::Mode::Country);
    }
    /* The About cell can either be found at index k_indexOfExamModeCell + 1 or
     * k_indexOfExamModeCell + 2, depending on whether there is a Pop-Up cell.
     * Since the Pop-Up cell has been handled above, we can use those two
     * indices for the About cell. */
    ViewController * subControllers[k_indexOfAboutCell + 2] = {
      &m_preferencesController,
      &m_displayModeController,
      &m_preferencesController,
      &m_preferencesController,
      nullptr, //&m_brightnessController
      &m_preferencesController,
      &m_localizationController,
      &m_localizationController,
      &m_examModeController,
      &m_aboutController,
      &m_aboutController
    };
    ViewController * selectedSubController = subControllers[rowIndex];
    assert(selectedSubController);
    if (model()->childAtIndex(rowIndex)->numberOfChildren() != 0) {
      static_cast<GenericSubController *>(selectedSubController)->setMessageTreeModel(model()->childAtIndex(rowIndex));
    }
    stackController()->push(selectedSubController);
    return true;
  }

  return false;
}

int MainController::numberOfRows() const {
  return model()->numberOfChildren();
};

KDCoordinate MainController::rowHeight(int index) {
  if (index == k_indexOfBrightnessCell) {
    willDisplayCellForIndex((HighlightCell *)&m_brightnessCell, index);
    return m_brightnessCell.minimalSizeForOptimalDisplay().height();
  }
  // if (index == k_indexOfLanguageCell) {
  //   MessageTableCellWithChevronAndMessage tempCell = MessageTableCellWithChevronAndMessage();
  //   willDisplayCellForIndex((HighlightCell *)&tempCell, index);
  //   return tempCell.minimalSizeForOptimalDisplay().height();
  // }
  // if (index == k_indexOfCountryCell) {
  //   MessageTableCellWithChevronAndMessage tempCell = MessageTableCellWithChevronAndMessage();
  //   willDisplayCellForIndex((HighlightCell *)&tempCell, index);
  //   return tempCell.minimalSizeForOptimalDisplay().height();
  // }
  if (hasPrompt() && index == k_indexOfPopUpCell) {
    willDisplayCellForIndex((HighlightCell *)&m_popUpCell, index);
    return m_popUpCell.minimalSizeForOptimalDisplay().height();
  }
  MessageTableCellWithChevronAndMessage tempCell = MessageTableCellWithChevronAndMessage();
  // tempCell.setMessageFont(KDFont::LargeFont);
  willDisplayCellForIndex((HighlightCell *)&tempCell, index);
  return tempCell.minimalSizeForOptimalDisplay().height();
#if 0
  if (j == k_indexOfBrightnessCell) {
    return Metric::ParameterCellHeight + CellWithSeparator::k_margin;
  }
  return Metric::ParameterCellHeight;
#endif
}

KDCoordinate MainController::cumulatedHeightFromIndex(int j) {
  // if (j > k_indexOfBrightnessCell) {
  //   return ListViewDataSource::cumulatedHeightFromIndex(j) + CellWithSeparator::k_margin;
  // }
  return ListViewDataSource::cumulatedHeightFromIndex(j);
#if 0
  KDCoordinate height = j * rowHeight(0);
  if (j > k_indexOfBrightnessCell) {
    height += CellWithSeparator::k_margin;
  }
  return height;
#endif
}

int MainController::indexFromCumulatedHeight(KDCoordinate offsetY) {
  // if (offsetY < ) {
  //   return ListViewDataSource::indexFromCumulatedHeight(offsetY);
  // }
  return ListViewDataSource::indexFromCumulatedHeight(offsetY);// - CellWithSeparator::k_margin);
#if 0
  if (offsetY < rowHeight(0)*k_indexOfBrightnessCell + CellWithSeparator::k_margin) {
    return offsetY/rowHeight(0);
  }
  return (offsetY - CellWithSeparator::k_margin)/rowHeight(0);
#endif
}

HighlightCell * MainController::reusableCell(int index, int type) {
  assert(index >= 0);
  if (type == 0) {
    assert(index < k_numberOfSimpleChevronCells);
    return &m_cells[index];
  }
  assert(index == 0);
  if (type == 2) {
    return &m_popUpCell;
  }
  assert(type == 1);
  return &m_brightnessCell;
}

int MainController::reusableCellCount(int type) {
  if (type == 0) {
    return k_numberOfSimpleChevronCells;
  }
  return 1;
}

int MainController::typeAtLocation(int i, int j) {
  if (j == k_indexOfBrightnessCell) {
    return 1;
  }
  if (hasPrompt() && j == k_indexOfPopUpCell) {
    return 2;
  }
  return 0;
}

void MainController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  GlobalPreferences * globalPreferences = GlobalPreferences::sharedGlobalPreferences();
  Preferences * preferences = Preferences::sharedPreferences();
  I18n::Message title = model()->childAtIndex(index)->label();
  if (index == k_indexOfBrightnessCell) {
    MessageTableCellWithGaugeWithSeparator * myGaugeCell = (MessageTableCellWithGaugeWithSeparator *)cell;
    myGaugeCell->setMessage(title);
    GaugeView * myGauge = (GaugeView *)myGaugeCell->accessoryView();
    myGauge->setLevel((float)globalPreferences->brightnessLevel()/(float)Ion::Backlight::MaxBrightness);
    return;
  }
  MessageTableCell * myCell = (MessageTableCell *)cell;
  myCell->setMessage(title);
  if (index == k_indexOfLanguageCell) {
    int index = (int)(globalPreferences->language());
    static_cast<MessageTableCellWithChevronAndMessage *>(cell)->setSubtitle(I18n::LanguageNames[index]);
    return;
  }
  if (index == k_indexOfCountryCell) {
    int index = (int)(globalPreferences->country());
    static_cast<MessageTableCellWithChevronAndMessage *>(cell)->setSubtitle(I18n::CountryNames[index]);
    return;
  }
  if (hasPrompt() && index == k_indexOfPopUpCell) {
    MessageTableCellWithSwitch * mySwitchCell = (MessageTableCellWithSwitch *)cell;
    SwitchView * mySwitch = (SwitchView *)mySwitchCell->accessoryView();
    mySwitch->setState(globalPreferences->showPopUp());
    return;
  }
  MessageTableCellWithChevronAndMessage * myTextCell = (MessageTableCellWithChevronAndMessage *)cell;
  // myTextCell->setMessageFont(KDFont::LargeFont);
  int childIndex = -1;
  switch (index) {
    case k_indexOfAngleUnitCell:
      childIndex = (int)preferences->angleUnit();
      break;
    case k_indexOfDisplayModeCell:
      childIndex = (int)preferences->displayMode();
      break;
    case k_indexOfEditionModeCell:
      childIndex = (int)preferences->editionMode();
      break;
    case k_indexOfComplexFormatCell:
      childIndex = (int)preferences->complexFormat();
      break;
    case k_indexOfFontCell:
      childIndex = GlobalPreferences::sharedGlobalPreferences()->font() == KDFont::LargeFont ? 0 : 1;
      break;
  }
  I18n::Message message = childIndex >= 0 ? model()->childAtIndex(index)->childAtIndex(childIndex)->label() : I18n::Message::Default;
  myTextCell->setSubtitle(message);
}

void MainController::viewWillAppear() {
  ViewController::viewWillAppear();
  m_selectableTableView.reloadData();
}

const SettingsMessageTree * MainController::model() {
  return &s_model;
}

StackViewController * MainController::stackController() const {
  return (StackViewController *)parentResponder();
}

}
