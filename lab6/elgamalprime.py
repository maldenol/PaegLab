def SieveOfEratosthenes(n):
    prime = [True for i in range(n+1)]
    p = 2
    while (p * p <= n):
        if (prime[p] == True):
            for i in range(p * p, n+1, p):
                prime[i] = False
        p += 1
 
    primes = []
    for p in range(2, n+1):
        if prime[p]:
            primes.append(p)
    return primes

primes = SieveOfEratosthenes(10000000)
primes.reverse()

for prime1 in primes:
    for prime2 in primes:
        if (prime1 - 1) % prime2 == 0:
            print((prime1, prime2))
