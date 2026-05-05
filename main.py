import pandas as pd
import matplotlib.pyplot as plt

# Вставка
df = pd.read_csv('insert_comparison.csv')
df['InsertUnsorted_sec'] *= 1e6
df['InsertSorted_sec'] *= 1e6
plt.plot(df['N'], df['InsertUnsorted_sec'], 'o-', label='Unsorted')
plt.plot(df['N'], df['InsertSorted_sec'], 's-', label='Sorted')
plt.xlabel('N')
plt.ylabel('microseconds')
plt.title('Insertion performance')
plt.legend()
plt.grid()
plt.savefig('insert_compare.png')
plt.show()

# Поиск
df = pd.read_csv('find_comparison.csv')
df['FindUnsorted_sec'] *= 1e6
df['FindSorted_sec'] *= 1e6
plt.plot(df['N'], df['FindUnsorted_sec'], 'o-', label='Unsorted')
plt.plot(df['N'], df['FindSorted_sec'], 's-', label='Sorted')
plt.xlabel('N')
plt.ylabel('microseconds')
plt.title('Find performance')
plt.legend()
plt.grid()
plt.savefig('find_compare.png')
plt.show()