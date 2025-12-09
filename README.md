## Feature Overview

The **Transactions module** enables performing financial operations on client accounts while ensuring data integrity and user confirmation.

All operations directly update client balances and are **persistently saved to the file system**.

---

## Transactions Supported

- ✅ **Deposit** – Add money to a client’s account
- ✅ **Withdraw** – Deduct money with balance validation
- ✅ **Total Balances** – Display all client balances and calculate the overall total
- ✅ Separate transactions menu with smooth navigation back to the main menu

---

## Key Implementations & Concepts

- Balance validation to prevent withdrawing more than the available amount
- Real-time update of client balances after each transaction
- Reusable functions for deposit, withdraw, and balance calculation
- File overwrite technique to ensure data consistency
- Menu control using **Enums** and clean user interaction flow
