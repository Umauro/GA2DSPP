import random as ran

for i in range(2,20):
    instancia = open(str(i)+".TXT", "w")
    instancia.write(str(i)+"\n")
    instancia.write(str(5)+"\n")
    for j in range(0, i):
        instancia.write(str(j+1) +" " +str(ran.randint(1,5)) + " " + str(ran.randint(1, 5))+"\n")  
    instancia.close()
