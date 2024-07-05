import os
import subprocess
import shutil

# esegui file 
def esegui_file_exe(percorso_exe):
# Verifica se il percorso esiste
    if not os.path.exists(percorso_exe):
        print("Il percorso specificato non esiste.")
        return
    
    # Verifica se il percorso è un file .exe
    if not percorso_exe.endswith(".exe"):
        print("Il percorso specificato non punta a un file .exe.")
        return
    
    try:
        # Esegui il file .exe utilizzando subprocess.run()
        subprocess.run(percorso_exe)
    except Exception as e:
        print("Si è verificato un errore durante l'esecuzione del file .exe:", str(e))


#modifica input 
def modifica_parametro(file_path, parametro, nuovo_valore):
    # Leggi il contenuto del file
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Cerca il parametro da modificare
    for i, line in enumerate(lines):
        if parametro in line:
            # Trovato il parametro, modifica il valore
            lines[i] = line.split()[0] + "  " + str(nuovo_valore) + "\n"
            break

    # Scrivi il file con il parametro modificato
    with open(file_path, 'w') as file:
        file.writelines(lines)


def copia_e_incolla_file(origine, destinazione):
    try:
        # Copia il file dalla cartella di origine alla cartella di destinazione
        shutil.copy2(origine, destinazione)
    except Exception as e:
        print(f"Si è verificato un errore durante la copia del file: {str(e)}")

def crea_cartella(percorso, nome_cartella):
    try:
        # Crea la cartella nel percorso specificato
        percorso_cartella = os.path.join(percorso, nome_cartella)
        os.makedirs(percorso_cartella, exist_ok=True)
    except Exception as e:
        print(f"Si è verificato un errore durante la creazione della cartella: {str(e)}")



#//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#METROPOLIS E GIBBS (Sim_type = 2,3)
#//////////////////////////////////////////////////////////////////////////////////////////////////////////////
T = [1.56,1.99,0.95]
rho = [1.1, 0.8, 0.05]
r_cut= [2.2, 2.5, 5.0]
file_path = "/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_7/Esercizio_1-2/INPUT/input.dat"
file = "./simulator.exe"  # Specifica il file .exe

for i in range(len(T)):
    modifica_parametro(file_path, "TEMP", T)
    esegui_file_exe(file)
    nome_out = ["Solid", "Liquid", "Gas"]
    k=0
    # PRIMA ESECUZIONE
    for i in nome_out:
        nome = f"SIM{k}"
        crea_cartella(f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0",nome)
        origine_file = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{i}"  # Specifica il percorso del file di origine
        destinazione_cartella = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0/SIM{k}"  # Specifica il percorso della cartella di destinazione
        copia_e_incolla_file(origine_file, destinazione_cartella)

    modifica_parametro(file_path, "RESTART", 1)
    while T < 3.0:
        T+=0.0625
        k+=1
        modifica_parametro(file_path, "TEMP", T)

        #IMPOSTO CONFIGURAZIONI INIZIALI 
        origine_file = "/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/CONFIG/config.spin"  # Specifica il percorso del file di origine
        destinazione_cartella = "/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/INPUT/CONFIG"  # Specifica il percorso della cartella di destinazione
        copia_e_incolla_file(origine_file, destinazione_cartella)

        #ESECUZIONE 
        esegui_file_exe(file)

        #SPOSTO GLI OUTPUT NELLE RISPETTIVE CARTELLE
        for i in nome_out:
            nome = f"SIM{k}"
            crea_cartella(f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0",nome)
            origine_file = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{i}"  # Specifica il percorso del file di origine
            destinazione_cartella = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0/SIM{k}"  # Specifica il percorso della cartella di destinazione
            copia_e_incolla_file(origine_file, destinazione_cartella)

    #MAGNETIZZAZIONE 
    T = 0.5
    modifica_parametro(file_path, "TEMP", T)
    modifica_parametro(file_path, "SIMULATION_TYPE", f"{Sim_type}      1.0     0.02")
    modifica_parametro(file_path, "RESTART", 0)
    #TORNO ALLE CONDIZIONI INIZIALI 
    esegui_file_exe(file)
    k=0

    # PRIMA ESECUZIONE
    nome_mag = "magnetization.dat"
    nome = f"SIM{k}"
    crea_cartella(f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0.02",nome)
    origine_file = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_mag}"  # Specifica il percorso del file di origine
    destinazione_cartella = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0.02/SIM{k}"  # Specifica il percorso della cartella di destinazione
    copia_e_incolla_file(origine_file, destinazione_cartella)

    modifica_parametro(file_path, "RESTART", 1)
    while T < 3.0:
        T+=0.0625
        k+=1
        modifica_parametro(file_path, "TEMP", T)

        #IMPOSTO CONFIGURAZIONI INIZIALI 
        origine_file = "/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/CONFIG/config.spin"  # Specifica il percorso del file di origine
        destinazione_cartella = "/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/INPUT/CONFIG"  # Specifica il percorso della cartella di destinazione
        copia_e_incolla_file(origine_file, destinazione_cartella)

        #ESECUZIONE 
        esegui_file_exe(file)

        #SPOSTO GLI OUTPUT NELLE RISPETTIVE CARTELLE
        nome = f"SIM{k}"
        crea_cartella(f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0.02",nome)
        origine_file = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_mag}"  # Specifica il percorso del file di origine
        destinazione_cartella = f"/mnt/c/Users/genty/OneDrive/Documenti/UNI/Simulazione numerica/Lab_6/Esercizio_1/OUTPUT/{nome_sim[Sim_type-2]}/H=0.02/SIM{k}"  # Specifica il percorso della cartella di destinazione
        copia_e_incolla_file(origine_file, destinazione_cartella)
        
    Sim_type+=1




