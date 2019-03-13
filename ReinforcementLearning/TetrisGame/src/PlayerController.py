from py4j.java_gateway import JavaGateway
gateway = JavaGateway()
rl_player = gateway.entry_point
value = rl_player.testMove(1)
print(value)