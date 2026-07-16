--- The InterfaceService acts for the User Interface (UI).
---
--- @class Interface
local Interface = {}

--- Open the UI View identified by this title.
--- @param title string
function Interface.OpenView(title) end

--- Open a dialogue with a certain title.
--- @param dialogue string
function Interface.OpenDialogue(dialogue) end

--- Get a reference to the current UI View.
--- @param title string
function Interface.GetCurrentView() end

return Interface
