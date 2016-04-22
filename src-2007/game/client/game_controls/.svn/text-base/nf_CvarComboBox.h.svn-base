

#ifndef COMMANDCOMBOBOX_H
#define COMMANDCOMBOBOX_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/ComboBox.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Panel.h>
#include "UtlVector.h"

class CCvarComboBox : public vgui::ComboBox
{
public:
	CCvarComboBox(vgui::Panel *parent, const char *panelName);
	~CCvarComboBox();

	virtual void DeleteAllItems();
	virtual void AddItem(char const *text, char const *engineCommand);
	virtual void ActivateItem(int itemIndex);
	const char *GetActiveItemCommand();

	void SetInitialItem(int itemIndex);

	void			ApplyChanges();
	void			Reset();
	bool			HasBeenModified();

	enum
	{
		MAX_NAME_LEN = 256,
		MAX_COMMAND_LEN = 256
	};

	DECLARE_PANELMAP();

private:
	typedef vgui::ComboBox BaseClass;

	void OnTextChanged( char const *text );

	struct COMMANDITEM
	{
		char			name[ MAX_NAME_LEN ];
		char			command[ MAX_COMMAND_LEN ];
		int				comboBoxID;
	};

	CUtlVector< COMMANDITEM >	m_Items;
	int		m_iCurrentSelection;
	int		m_iStartSelection;
};

#endif // COMMANDCOMBOBOX_H
