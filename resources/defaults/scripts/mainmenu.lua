function open()
    view:ChangeFocus("play")
end

function play()
    print("pressed play")
    World.SetRoom("map")
end

function quit()
    Game.Close()
end
