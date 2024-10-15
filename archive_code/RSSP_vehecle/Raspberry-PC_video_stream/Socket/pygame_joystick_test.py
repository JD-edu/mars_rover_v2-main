import pygame 

pygame.init()

window = pygame.display.set_mode((640, 480))
pygame.display.set_caption("Joytick test")

clock = pygame.time.Clock()
gamepads = []

for a in range(0, pygame.joystick.get_count()):
    print( pygame.joystick.get_count())
    gamepads.append(pygame.joystick.Joystick(a))
    gamepads[-1].init()
    print(gamepads[-1].get_name())

run = True
while run:
    clock.tick(30)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
    if event.type == pygame.JOYBUTTONDOWN:
        print(event.button)

    if event.type == pygame.JOYAXISMOTION:
        x1 = gamepads[-1].get_axis(0)
        x2 = gamepads[-1].get_axis(1)
        x3 = gamepads[-1].get_axis(2)
        x4 = gamepads[-1].get_axis(3)
        print(x1, x2, x3, x4)
    
    pygame.display.update()
pygame.quit()


    