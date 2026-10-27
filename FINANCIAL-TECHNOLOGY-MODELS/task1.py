""" 
Vilnius University
Financial Technology Models
Task 1: RSA Cypher
@author Tayri Amador Guillén
@date 2026-10-22
@brief Solution of RSA Cypher
"""

import random

# --- ANSI Color Codes ---
BOLD = '\033[1m'
RED = '\033[91m'
CYAN = '\033[96m'
RESET = '\033[0m'

def fermat_test(n, k=5):
    if n <= 1:
        return False
    if n <= 3:
        return True
    
    for _ in range(k):
        a = random.randint(2, n - 2)
        if pow(a, n - 1, n) != 1:
            return False
    return True

def extended_euclidean(a, b):
    if a == 0:
        return b, 0, 1
    gcd, x1, y1 = extended_euclidean(b % a, a)
    x = y1 - (b // a) * x1
    y = x1
    return gcd, x, y

def modular_inverse(e, phi):
    gcd, x, y = extended_euclidean(e, phi)
    if gcd != 1:
        raise Exception('No modular inverse exists. (e and phi are not coprime)')
    return x % phi

def rsa_encrypt(m, e, n):
    return pow(m, e, n)

def rsa_decrypt(c, d, n):
    return pow(c, d, n)

def execute_rsa(p, q, e, m):
    p_is_prime = fermat_test(p)
    q_is_prime = fermat_test(q)

    if not p_is_prime or not q_is_prime:
        print(f"\n{BOLD}{RED}[!] ERROR: Primality check failed!{RESET}")
        if not p_is_prime:
            print(f"{RED} -> 'p' ({p}) is NOT a prime number.{RESET}")
        if not q_is_prime:
            print(f"{RED} -> 'q' ({q}) is NOT a prime number.{RESET}")
        print(f"Please provide valid prime numbers to continue.")
        return 

    n = p * q
    phi_n = (p - 1) * (q - 1)
    
    try:
        d = modular_inverse(e, phi_n)
    except Exception as error:
        print(f"\n{BOLD}{RED}[!] MATHEMATICAL ERROR: {error}{RESET}")
        return

    print(f"\n{BOLD}{CYAN}--- Key Parameters ---{RESET}")
    print(f"Modulus (n) = p * q = {n}")
    print(f"Phi(n) = (p-1)*(q-1) = {phi_n}")
    print(f"Public key (e) = {e}")
    print(f"Private key (d) = {d}")

    print(f"\n{BOLD}{CYAN}--- Ciphertext Calculation ---{RESET}")
    print(f"Original message (m) = {m}")
    
    c = rsa_encrypt(m, e, n)
    print(f"Ciphertext (C) = {c}")
    
    m_decrypted = rsa_decrypt(c, d, n)
    print(f"Decrypted message for verification: {m_decrypted}")


if __name__ == '__main__':
    print(f"{BOLD}=== RSA CYPHER SIMULATOR ==={RESET}")
    print("1. Execute default task1 values")
    print("2. Enter custom values manually")
    
    option = input("\nChoose an option (1 or 2): ")
    
    if option == '1':
        print(f"\n[Executing default values...]")
        p = 646253
        q = 953347
        e = 508447791809
        m = 12345678
        execute_rsa(p, q, e, m)
        
    elif option == '2':
        print(f"\n[Manual Mode]")
        try:
            p = int(input("Enter the value for p (must be prime): "))
            q = int(input("Enter the value for q (must be prime): "))
            e = int(input("Enter the value for e: "))
            m = int(input("Enter the message m (as an integer): "))
            
            execute_rsa(p, q, e, m)
        except ValueError:
            print(f"\n{BOLD}{RED}ERROR: Invalid input. Make sure to enter only integers.{RESET}")
            
    else:
        print(f"\n{RED}Invalid option. Please restart the program and enter 1 or 2.{RESET}")