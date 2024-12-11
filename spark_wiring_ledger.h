/*
 * Copyright (c) 2023 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// This is a modified version that only contains LedgerData

#include <functional>
#include <initializer_list>
#include <utility>

#include "spark_wiring_variant.h"

namespace particle {

/**
 * Ledger data.
 *
 * This class provides a subset of methods of the `Variant` class that are relevant to map operations.
 */
class LedgerData {
public:
    /**
     * Data entry.
     *
     * A data entry is an `std::pair` where `first` is the entry name (a `String`) and `second` is
     * the entry value (a `Variant`).
     */
    using Entry = VariantMap::Entry;

    /**
     * Construct empty ledger data.
     */
    LedgerData() :
            v_(VariantMap()) {
    }

    /**
     * Construct ledger data from a `Variant`.
     *
     * If the `Variant` is not a map, empty ledger data is constructed.
     *
     * @param var `Variant` value.
     */
    LedgerData(Variant var) {
        if (var.isMap()) {
            v_ = std::move(var);
        } else {
            v_ = var.toMap();
        }
    }

    /**
     * Construct ledger data from an initializer list.
     *
     * Example usage:
     * ```
     * LedgerData data = { { "key1", "value1" }, { "key2", 2 } };
     * ```
     *
     * @param entries Entries.
     */
    LedgerData(std::initializer_list<Entry> entries) :
            v_(VariantMap(entries)) {
    }

    /**
     * Copy constructor.
     *
     * @param data Ledger data to copy.
     */
    LedgerData(const LedgerData& data) :
            v_(data.v_) {
    }

    /**
     * Move constructor.
     *
     * @param data Ledger data to move from.
     */
    LedgerData(LedgerData&& data) :
            LedgerData() {
        swap(*this, data);
    }

    ///@{
    /**
     * Set the value of an entry.
     *
     * @param name Entry name.
     * @param val Entry value.
     * @return `true` if the entry value was set, or `false` on a memory allocation error.
     */
    bool set(const char* name, Variant val) {
        return v_.set(name, std::move(val));
    }

    bool set(const String& name, Variant val) {
        return v_.set(name, std::move(val));
    }

    bool set(String&& name, Variant val) {
        return v_.set(std::move(name), std::move(val));
    }
    ///@}

    ///@{
    /**
     * Remove an entry.
     *
     * @param name Entry name.
     * @return `true` if the entry was removed, otherwise `false`.
     */
    bool remove(const char* name) {
        return v_.remove(name);
    }

    bool remove(const String& name) {
        return v_.remove(name);
    }
    ///@}

    ///@{
    /**
     * Get the value of an entry.
     *
     * This method is inefficient for complex value types, such as `String`, as it returns a copy of
     * the value. Use `operator[]` to get a reference to the value.
     *
     * A null `Variant` is returned if the entry doesn't exist.
     *
     * @param name Entry name.
     * @return Entry value.
     */
    Variant get(const char* name) const {
        return v_.get(name);
    }

    Variant get(const String& name) const {
        return v_.get(name);
    }
    ///@}

    ///@{
    /**
     * Check if an entry with the given name exists.
     *
     * @param name Entry name.
     * @return `true` if the entry exists, otherwise `false`.
     */
    bool has(const char* name) const {
        return v_.has(name);
    }

    bool has(const String& name) const {
        return v_.has(name);
    }
    ///@}

    /**
     * Get all entries of the ledger data.
     *
     * Example usage:
     * ```
     * LedgerData data = myLedger.get();
     * for (const LedgerData::Entry& entry: data.entries()) {
     *     const String& name = entry.first;
     *     const Variant& value = entry.second;
     *     Log.info("%s: %s", name.c_str(), value.toString().c_str());
     * }
     * ```
     *
     * @return Entries.
     */
    const Vector<Entry>& entries() const {
        return variantMap().entries();
    }

    /**
     * Get the number of entries stored in the ledger data.
     *
     * @return Number of entries.
     */
    int size() const {
        return v_.size();
    }

    /**
     * Get the number of entries that can be stored without reallocating memory.
     *
     * @return Number of entries.
     */
    int capacity() const {
        return variantMap().capacity();
    }

    /**
     * Reserve memory for the specified number of entries.
     *
     * @return `true` on success, or `false` on a memory allocation error.
     */
    bool reserve(int capacity) {
        return variantMap().reserve(capacity);
    }

    /**
     * Reduce the capacity of the ledger data to its actual size.
     *
     * @return `true` on success, or `false` on a memory allocation error.
     */
    bool trimToSize() {
        return variantMap().trimToSize();
    }

    /**
     * Check if the ledger data is empty.
     *
     * @return `true` if the data is empty, otherwise `false`.
     */
    bool isEmpty() const {
        return v_.isEmpty();
    }

    /**
     * Serialize the ledger data as JSON.
     *
     * @return JSON document.
     */
    String toJSON() const {
        return v_.toJSON();
    }

    ///@{
    /**
     * Get a reference to the `VariantMap` containing the entries of the ledger data.
     *
     * @return Reference to the `VariantMap`.
     */
    VariantMap& variantMap() {
        return v_.value<VariantMap>();
    }

    const VariantMap& variantMap() const {
        return v_.value<VariantMap>();
    }
    ///@}

    ///@{
    /**
     * Get a reference to the underlying `Variant`.
     *
     * @return Reference to the `Variant`.
     */
    Variant& variant() {
        return v_;
    }

    const Variant& variant() const {
        return v_;
    }
    ///@}

    ///@{
    /**
     * Get a reference to the entry value.
     *
     * The entry is created if it doesn't exist.
     *
     * @note The device will panic if it fails to allocate memory for the new entry. Use `set()`
     * or the methods provided by `VariantMap` if you need more control over how memory allocation
     * errors are handled.
     *
     * @param name Entry name.
     * @return Entry value.
     */
    Variant& operator[](const char* name) {
        return v_[name];
    }

    Variant& operator[](const String& name) {
        return v_[name];
    }

    Variant& operator[](String&& name) {
        return v_[std::move(name)];
    }
    ///@}

    /**
     * Assignment operator.
     *
     * @param data Ledger data to assign from.
     * @return This ledger data.
     */
    LedgerData& operator=(LedgerData data) {
        swap(*this, data);
        return *this;
    }

    /**
     * Comparison operators.
     *
     * Two instances of ledger data are equal if they contain equal sets of entries.
     */
    ///@{
    bool operator==(const LedgerData& data) const {
        return v_ == data.v_;
    }

    bool operator!=(const LedgerData& data) const {
        return v_ != data.v_;
    }

    bool operator==(const Variant& var) const {
        return v_ == var;
    }

    bool operator!=(const Variant& var) const {
        return v_ != var;
    }
    ///@}

    /**
     * Parse ledger data from JSON.
     *
     * If the root element of the JSON document is not an object, empty ledger data is returned.
     *
     * @param json JSON document.
     * @return Ledger data.
     */
    static LedgerData fromJSON(const char* json) {
        return Variant::fromJSON(json);
    }

    /**
     * Convert a JSON value to ledger data.
     *
     * If the JSON value is not an object, empty ledger data is returned.
     *
     * @param Val JSON value.
     * @return Ledger data.
     */
    static LedgerData fromJSON(const JSONValue& val) {
        return Variant::fromJSON(val);
    }

    friend void swap(LedgerData& data1, LedgerData& data2) {
        using std::swap; // For ADL
        swap(data1.v_, data2.v_);
    }

private:
    Variant v_;
};

inline bool operator==(const Variant& var, const LedgerData& data) {
    return data == var;
}

inline bool operator!=(const Variant& var, const LedgerData& data) {
    return data != var;
}

} // namespace particle
